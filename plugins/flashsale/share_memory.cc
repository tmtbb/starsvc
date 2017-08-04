//share_memmory.cc

#include "logic/logic_comm.h"
#include "share_memory.h"

namespace flash_logic {

#define MAXSIZE 1000

CShareMemory::CShareMemory(){
	if( !Init() ){
		LOG_MSG("CShareMemory init error.");
		assert(0);
	}
}

CShareMemory::~CShareMemory(){
	Destroy();
}

bool CShareMemory::Init(){
	if((m_key = ftok("/etc/passwd", 0x66)) < 0)
	{
		LOG_MSG("create key failure, CShareMemory init error.");
		return false;
	}
	//获得信号量
	//if((m_semid=semget(m_key,1,0666|IPC_CREAT)) < 0){
	if((m_semid=semget((key_t)6688,1,0666|IPC_CREAT)) < 0){
		LOG_MSG2("semget failure, semid[%d]", m_semid);
		return false;
	}
	LOG_MSG2("semget semid[%d]", m_semid);

	union semun arg_sem;
	arg_sem.val=1;
	if(semctl(m_semid,0,SETVAL,arg_sem)==-1){
		LOG_MSG2("semctl error, semid[%d]", m_semid);
		semctl(m_semid,0,IPC_RMID,arg_sem);
	}
	
	return true;	
}

bool CShareMemory::AttachMemry(int32 flag){
	//创建共享内存
	int shmflg;
	if(flag==0){
		shmflg = IPC_CREAT|IPC_EXCL|0644;
		m_shmid = shmget(m_key, sizeof(flash_logic::PulishStar)*MAXSIZE, shmflg);
	}else{
		m_shmid = shmget(m_key,0,0);
	}
	//m_shmid = shmget(m_key, sizeof(flash_logic::PulishStar)*MAXSIZE, shmflg);
  if (m_shmid == -1) 
  {
    LOG_MSG2("shmget failed, shmid[%d].", m_shmid);
    return false;
  }
  LOG_MSG2("shmget shmid[%d]", m_shmid);
  //将共享内存连接到当前进程的地址上
	m_shmAdd = shmat(m_shmid,NULL,0);
	if(m_shmAdd == (void *)-1){
		LOG_MSG("shmat error.");
		return false;
	}
	LOG_MSG2("shmat addr[%p]", m_shmAdd);

	return true;
}

void CShareMemory::Destroy(){
	if(shmdt(m_shmAdd) < 0) {
		LOG_MSG("shmdt()");
		exit(1);
  }

  if(shmctl(m_shmid, IPC_RMID, 0) == -1){
  	LOG_MSG("shmctl(IPC_RMID) failed.");
  	exit(1);
  }
}

/***对信号量数组semnum编号的信号量做P操作***/
int CShareMemory::P(int semid, int semnum){
	struct sembuf sops={semnum, -1, SEM_UNDO};
	return (semop(semid,&sops,1));
}

/***对信号量数组semnum编号的信号量做V操作***/
int CShareMemory::V(int semid, int semnum){
	struct sembuf sops={semnum, +1, SEM_UNDO};
	return (semop(semid,&sops,1));
}

void CShareMemory::SetStarToShareMemory(PUBLISH_STAR_MAP& starmap){
	P(m_semid, 0);
	int size = starmap.size();
	if(size > MAXSIZE){
		LOG_MSG2("starmap size over[%d].", starmap.size());
		assert(0);
		return ;
	}
	int* p_shmadd = (int*)m_shmAdd;
	*(p_shmadd++) = size;
	flash_logic::PulishStar* pstar = (flash_logic::PulishStar*)p_shmadd;
	//LOG_MSG2("begin:[%p],after int[%p],class begin[%p],+1 addr[%p]",
	//					m_shmAdd, p_shmadd, pstar, (pstar+1));
	
	PUBLISH_STAR_MAP::iterator it = starmap.begin();
	for(int i=0; it != starmap.end() || i<size; ++it,++i){
		*(pstar+i) = it->second;
	}
	V(m_semid, 0);
}

void CShareMemory::SetStarToShareMemory(flash_logic::PulishStar& star){
	P(m_semid, 0);
	int* p_shmadd = (int*)m_shmAdd;
	int size = *(p_shmadd++);
	flash_logic::PulishStar* pstar = (flash_logic::PulishStar*)p_shmadd;
	for(int i=0; i<size; ++i){
		if(strcmp((pstar+i)->symbol(), star.symbol()) == 0){
			*(pstar+i) = star;
			break;
		}
	}
	V(m_semid, 0);
}

bool CShareMemory::GetStarfromShareMemory(const std::string& symbol, 
																flash_logic::PulishStar& star){
	bool r = false;
	int* p_shmadd = (int*)m_shmAdd;
	int size = *(p_shmadd++);
	flash_logic::PulishStar* pstar = (flash_logic::PulishStar*)p_shmadd;
	for(int i=0; i<size; ++i){
		if(strcmp((pstar+i)->symbol(), symbol.c_str()) == 0){
			star = *(pstar+i);
			r = true;
			break;
		}
	}
	return r;
}

int CShareMemory::GetStarfromShareMemory(PUBLISH_STAR_MAP& starmap){
	int* p_shmadd = (int*)m_shmAdd;
	int size = *(p_shmadd++);
	LOG_MSG2("GetStarfromShareMemory size[%d]", size);
	flash_logic::PulishStar* pstar = (flash_logic::PulishStar*)p_shmadd;
	//LOG_MSG2("GetStarfromShareMemory begin[%p],addr[%p],+1[%p]", m_shmAdd,pstar,(pstar+1));
	for(int i=0; i<size; ++i){
		starmap[(pstar+i)->symbol()] = *(pstar+i);
	}
	return size;
}

// 之后必须调用DeductStarLastTime 或StarLastTimeFinish 之一
bool CShareMemory::StarLastTimeIsEnough(const std::string& symbol,const int64 amount){
	P(m_semid, 0);
	int* p_shmadd = (int*)m_shmAdd;
	int size = *(p_shmadd++);
	flash_logic::PulishStar* pstar = (flash_logic::PulishStar*)p_shmadd;
	for(int i=0; i<size; ++i){
		if(strcmp((pstar+i)->symbol(), symbol.c_str()) == 0){
			return amount <= (pstar+i)->publish_last_time();
		}
	}
	LOG_MSG2("Cann't find symbol[%s] in memory.", symbol.c_str());
	return false;
}

int64 CShareMemory::DeductStarLastTime(const std::string& symbol,const int64 amount){
	int* p_shmadd = (int*)m_shmAdd;
	int size = *(p_shmadd++);
	flash_logic::PulishStar* pstar = (flash_logic::PulishStar*)p_shmadd;
	for(int i=0; i<size; ++i){
		if(strcmp((pstar+i)->symbol(), symbol.c_str()) == 0){
			int64 last_time = (pstar+i)->publish_last_time() - amount;
			(pstar+i)->set_publish_last_time(last_time);
			V(m_semid, 0);
			return last_time;
		}
	}

	LOG_MSG2("Cann't find symbol[%s] in memory.", symbol.c_str());
	V(m_semid, 0);
	return -1;
}

bool CShareMemory::JudgeStarLastTimeFinish(const std::string& symbol){
	V(m_semid, 0);
	return true;
}

bool CShareMemory::DelStarFromShareMemory(const std::string& symbol){
/*	PUBLISH_STAR_MAP starmap = (*(PUBLISH_STAR_MAP*)m_shmAdd);
	PUBLISH_STAR_MAP::iterator it = starmap.find(symbol);
	if(it != starmap.end()){
		starmap.erase(it);
		return true;
	}
	return false;
	*/
	return true;
}

}  // namespace flash_logic

/*
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>//共享存储头文件
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>//利用信号量实现同步功能控制对共享内存的访问
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
	void *shm=NULL;//分配的共享内存的首地址

	//利用当前路径，创建key_t
	char ptr[50];
	if(getcwd(ptr,50)==NULL)
		printf("getcwd erro\n");
	printf("cwd=%s\n",ptr);

	//获得信号量
	int sig=semget(IPC_PRIVATE,1,0666|IPC_CREAT);
	if(sig==-1)
		printf("get sem erro\n");
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short *array;
	}arg_sem;
	arg_sem.val=1;
	if(semctl(sig,0,SETVAL,arg_sem)==-1)
		printf("信号量初始化失败\n");
	//分配资源
	struct sembuf sem,fsem;
	sem.sem_num=0;
	sem.sem_op=-1;
	sem.sem_flg=SEM_UNDO;
//	semop(sig,sem,1);
	//释放资源
	fsem.sem_num=0;
	fsem.sem_op=1;
	fsem.sem_flg=SEM_UNDO;
	//创建共享内存
	int shmid;
	shmid=shmget(IPC_PRIVATE,1024,0666);
	if(shmid==-1)
	{
		printf("get a shm erro\n");
		exit(0);
	}
	//将共享内存连接到当前进程的地址上
	shm=shmat(shmid,0,0);
	if(shm==(void *)-1)
		printf("shmat erro\n");
	printf("shm at %X\n",(int)shm);
	char buffer[BUFSIZ+1];//保存输入的文本
	
	//把共享内存，信号量标识符写入文件，以便客户端读取利用  sig  shmid
	FILE* fd;
	if((fd=fopen("log","w"))==NULL)
		printf("创建文件失败\n");
	if(fwrite(&sig,sizeof(int),1,fd)!=1)
		printf("fwrite sig erro\n");
	if(fwrite(&shmid,sizeof(int),1,fd)!=1)
		printf("fwrite shmid erro\n");
	fclose(fd);
	
	printf("sig:%d\n shmid:%d\n",sig,shmid);
	printf("bufsize:%d\n",BUFSIZ);
	char *ch;
	int b=0;
	while(1)
	{
		if(semop(sig,&sem,1)==-1)//分配资源出错
			printf("分配资源出错\n");
		else
		{
			printf("占据资源\n");
			//分配成功，写内存
			printf("Enter some data:");
			fgets(buffer,BUFSIZ,stdin);
			if(strncmp(buffer,"end",3)==0)
				b=1;	

			strncpy(shm,buffer,2048);
			ch=shm;
			while(*ch!='\0')
				printf("%c",*ch),ch++;
			//释放资源
			if(semop(sig,&fsem,1)==-1)
				printf("free sem erro\n");
			printf("释放资源\n");
			if(b)
				break;
		}
		usleep(100);
	}
	return 0;
}
*/


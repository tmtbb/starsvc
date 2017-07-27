//share_memory.h
#ifndef STAR_FLASH_SHARE_MEMMORY_H__
#define STAR_FLASH_SHARE_MEMMORY_H__

#include <sys/ipc.h>
#include <sys/shm.h>//共享存储头文件
#include <unistd.h>
#include <sys/sem.h>//利用信号量实现同步功能控制对共享内存的访问
#include <sys/stat.h>
#include <fcntl.h>
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "flash_info.h"


namespace flash_logic {

typedef std::map<std::string, flash_logic::PulishStar> PUBLISH_STAR_MAP;//key:symbol

union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};

class CShareMemory {
public:
	CShareMemory();
	virtual ~CShareMemory();

	//链接共享内存，flag:0 新建 1 链接
	bool AttachMemry(int32 flag);

	//写共享内存
	void SetStarToShareMemory(PUBLISH_STAR_MAP& starmap);
	void SetStarToShareMemory(flash_logic::PulishStar& star);

	//读共享内存
	bool GetStarfromShareMemory(const std::string& symbol, flash_logic::PulishStar& star);

	//读共享内存，全部
	int GetStarfromShareMemory(PUBLISH_STAR_MAP& starmap);

	//判断明星剩余时间， StarLastTimeIsEnough 之后必须调用DeductStarLastTime 或StarLastTimeFinish 之一
	bool StarLastTimeIsEnough(const std::string& symbol, const int64 amount);

	//扣减明星剩余时间
	int64 DeductStarLastTime(const std::string& symbol, const int64 amount);

	//明星剩余时间判断结束
	bool JudgeStarLastTimeFinish(const std::string& symbol);

	//删除内存数据
	bool DelStarFromShareMemory(const std::string& symbol);


private:
	bool Init();
	void Destroy();

	int P(int semid, int semnum);
	int V(int semid, int semnum);

private:
	void* m_shmAdd;//分配的共享内存的首地址
	key_t m_key;
	int m_semid;
	int m_shmid;
};

}  // namespace flash_logic

#endif



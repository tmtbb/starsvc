#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include "time.h"

#define PORT 16005

typedef signed char         int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;

struct PacketHead{
    int16 packet_length;
    int8  is_zip_encrypt;
    int8  type;
    int16 signature;
    int16 operate_code;
    int16 data_length;
    int32 timestamp;
    int64 session_id;
    int32 reserved;
};

void printhelp(){
    printf("**************************************************\n");
    printf("2 个参数，第一个操作码，第二个文件名\n");
    printf("eg:./clint 9005 9005.txt\n");

}
int main(int argv,char** args)
{
    if(argv != 3){
	printhelp();
	return 1;
    }
    int conndfd;
    struct sockaddr_in serverAddr;
    if(-1==(conndfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
        printf("Create Socket Error\n");
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr.sin_port=htons(PORT);
    if(-1==connect(conndfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)))
    {
        printf("Connetc Error\n");
        exit(0);
    }
    printf("Connect Success.Lets communicate!\n");

    struct PacketHead pack = {
	packet_length:10,
	is_zip_encrypt:0,
	type:1,
	signature:0,
	operate_code:9005,
	data_length:1,
	timestamp:time(NULL),
	session_id:1,
	reserved:0
    };
    //char str[] = "{\"name_value\": \"abcd\",\"accid_value\": \"123456\"}";
    pack.operate_code = atoi(args[1]);

    FILE *file = NULL;
    int lsize;
    char *str;
    file = fopen(args[2], "rb");
    if (file == NULL) 
    {
	    fputs("File error", stderr); exit(1);
    }
    fseek(file , 0 , SEEK_END); 
    lsize = ftell(file);
    rewind(file);
    str = (char*) malloc(sizeof(char) * lsize);  
    if (str == NULL) 
    {
	    fputs("Memory error", stderr); exit(2);
    }
    fread(str, 1, lsize, file);
    fclose(file);



    pack.packet_length = 26+strlen(str);
    pack.data_length = sizeof(str);
    char *buf = (char*)malloc(26+strlen(str));
    memcpy(buf,&pack,26);
    memcpy(buf+26,str,strlen(str));
    int strlength = 26+strlen(str);
    printf("send============%s\n",buf + 26);
    send(conndfd,buf,strlength,0);


    char rev[1024];
    memset(rev,0,sizeof(rev));
    int n = recv(conndfd,rev,1024,0);
    printf("Recv %d byte from Server\n",n);
    printf("%s\n",rev+26);
    close(conndfd);
    return 0;
}

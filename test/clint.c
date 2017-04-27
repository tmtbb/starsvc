#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>

#define PORT 16005

typedef signed char         int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

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

int main()
{
    int conndfd;
    struct sockaddr_in serverAddr;
    char buf[1024+1];
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
	is_zip_encrypt:1,
	type:1,
	signature:1,
	operate_code:1,
	data_length:1,
	timestamp:1,
	session_id:1,
	reserved:1
    };
    char str[] = "abcdefg";
    memcpy(buf,&pack,sizeof(pack));
    memcpy(buf+26,str,strlen(str));
    int strlength = sizeof(pack)+strlen(str);
    send(conndfd,buf,strlength,0);
    /*
    int16 head2 = 6;
    memcpy(buf,&head2,sizeof(head2));
    buf[2] = 'a';
    buf[3] = 'b';
    buf[4] = 'c';
    buf[5] = '\0';
    int n=send(conndfd,buf,6,0);
    */
    //memset(buf,0,sizeof(buf));
    //n=recv(conndfd,buf,1024,0);
    //printf("Recv %d byte from Server\n",n);
    //printf("%s\n",buf);
    close(conndfd);
    return 0;
}

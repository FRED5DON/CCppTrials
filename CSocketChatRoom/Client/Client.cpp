//
// Created by Fred Don on 2017/3/8.
//
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SOCKET_SERVER_PORT 8098
#define BUFFER_LEN 256


int main(){

    //定义本地socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);


    /**
     * 定义要连接的server的地址 端口号等
     */
    struct sockaddr_in server_socket_addr;
    server_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_socket_addr.sin_port = htons(SOCKET_SERVER_PORT);
    server_socket_addr.sin_family = AF_INET;


    //连接服务器 成功返回0，错误返回-1
    /**
        * @s 套接字
        * @名称  sockaddr 结构指针，包含要连接主机的地址和端口
        * @缓冲区长度
        * @returnInt 返回0成功 否则失败
        */
    if(connect(client_socket,(struct sockaddr *)&server_socket_addr, sizeof(struct sockaddr))!=0){
        printf("连接失败");
        exit(1);
    }

    char buffRecv[BUFFER_LEN];
    char buffSend[BUFFER_LEN];

    while(1){
        printf("请输入要发送的消息：\n");
        scanf("%s",buffSend);
        if(strcmp(buffSend,"quit")==0){
            break;
        }
        ssize_t sendLen = send(client_socket, buffSend, BUFFER_LEN, 0);
        if (sendLen<0){
            printf("发送失败");
        }

        ssize_t recvLen=recv(client_socket,buffRecv,BUFFER_LEN,0);
        if (recvLen<0){
            printf("接收失败");
            break;
        }
        printf("从服务端收到消息：%s\n",buffRecv);
    }
    close(client_socket);

    return 0;
}

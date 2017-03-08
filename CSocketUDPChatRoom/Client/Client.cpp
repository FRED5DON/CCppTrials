//
// Created by Fred Don on 2017/3/8.
//
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SOCKET_SERVER_PORT 8099
#define BUFFER_LEN 256


int main(){

    //定义本地socket
    int client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


    /**
     * 定义要连接的server的地址 端口号等
     */
    struct sockaddr_in server_socket_addr;
    server_socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_socket_addr.sin_port = htons(SOCKET_SERVER_PORT);
    server_socket_addr.sin_family = AF_INET;



    char buffRecv[BUFFER_LEN];
    char buffSend[BUFFER_LEN];

    socklen_t addrLen=sizeof(struct sockaddr_in);

    while(1){
        printf("请输入要发送的消息：\n");
        scanf("%s",buffSend);
        if(strcmp(buffSend,"quit")==0){
            break;
        }
        ssize_t sendLen = sendto(client_socket, buffSend, BUFFER_LEN, 0,(sockaddr *)&server_socket_addr,addrLen);
        if (sendLen<0){
            perror("发送失败");
        }
        ssize_t recvLen=recvfrom(client_socket,buffRecv,BUFFER_LEN,0,(sockaddr *)&server_socket_addr,&addrLen);

        if (recvLen<0){
            perror("接收失败");
            break;
        }
        //buffRecv[recvLen] = 0x00;
        printf("从服务端收到消息：%s\n",buffRecv);
    }
    close(client_socket);

    return 0;
}

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

//服务端
int main() {

    //创建套接字
    /**
     * @地址族 AF_INET 表示指定地址族
     * @套接字类型 SOCK_STREAM 表示流式套接字TCP 、 SOCKET_DGRAM 无连接数据包套接字（UDP）、SOCKET_RAW 原始套接字
     * @用户协议 0为不指定
     */
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //定义sockaddr_in
    /**
     * 定义server的地址 端口号等
     */
    struct sockaddr_in server_socket_addr;


    /* * * * * * * * * * * * * * *
     * 网络字节序是 big-endian 高位字节放置内存低地址
     * */

    /**
     * 将主机字节顺序转换为网络字节顺序
     *
     * 网络字节数据NBO（Network Byte Order）:从高到低的顺序存储
     *
     * htonl -> Host To Network Long
     * INADDR_ANY -> u_int32_t ，Server Ip 地址
     * @description 原因：数据的顺序是由cpu决定的，不同CPU数据高地位存储顺序也不同
     * @returntype __uint32_t
     *
     * inet_addr(const char *); ->将ip字符串转换为点分十进制
     */
    server_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /**
     *
     * 端口范围为0~65535 其中0~255为系统保留端口
     * 就是说如果你的端口号超过这个范围 会被htons舍去以上的位
     *
     * @returntype __uint16_t
     */
    server_socket_addr.sin_port = htons(SOCKET_SERVER_PORT);
    /**
     * 协议簇
     * AF_INET  TCP/IP协议
     * @注意 只有这个不是网络套接字顺序
     */
    server_socket_addr.sin_family = AF_INET;

    /**
     * @s 套接字
     * @名称  sockaddr 结构指针，包含地址和端口
     * @缓冲区长度
     * @returnInt 返回0成功 否则失败
     */
    int status = bind(server_socket, (struct sockaddr *) &server_socket_addr, sizeof(struct sockaddr));
    if (status != 0) {
        printf("Server 初始化失败");
        exit(0);
    }
    //监听端口
    /**
     * @s 套接字
     * @backlog 表示等待连接的最大队列长度
     * @descripion 如果backlog设置20，若此时有21个客户端同时发出连接请求，前20个会在队列等待，而其他的h会得到错误消息
     */
    if(listen(server_socket,20)<0){
        printf("Server 监听失败");
        exit(1);
    }

    /**
     * @s 套接字
     * @name  sockaddr 结构指针，包含地址和端口
     * @addrLen 用于接收参数的长度 socklen_t (unsigned int)
     */
     socklen_t addrLen=sizeof(struct sockaddr);
    int accept_connection = accept(server_socket, (struct sockaddr *) &server_socket_addr, &addrLen);

    if (accept_connection<0){
        printf("Server 连接失败");
        exit(1);
    }

    char buffRecv[BUFFER_LEN];
    char buffSend[BUFFER_LEN];


    printf("Server 初始化完成");
    //无限Loop 接收客户端请求、响应客户端请求
    while(1){
        /**
         * 从面向连接的套接字中接收数据
         * @s 套接字
         * @buff 接收数据的缓冲区
         * @len 缓冲区长度
         * @flags 表示函数的调用方式,一般设0 其他部分数值如下:>
         *          MSG_PEEK 表示查看传来的数据，在序列前段的数据会被复制一份到返回的缓冲区，但这个数据不会被移走
         *          MSG_OOB 表示用来处理Out Of Band数据，也就是外带数据。
         *          MSG_WAITALL 强迫接收到len 大小的数据后才能返回, 除非有错误或信号产生
         *          MSG_NOSIGNAL 此操作不愿被SIGPIPE 信号中断返回值成功则返回接收到的字符数, 失败返回-1
         */
        ssize_t recvLen=recv(accept_connection,buffRecv,BUFFER_LEN,0);

        if(strcmp(buffSend,"quitAll")==0){
            break;
        }
        if (recvLen<=0){
            printf("接收数据失败，程序错误\n");
            break;
        }
        printf("从客户端收到消息：%s\n",buffRecv);

        sprintf(buffSend,"》》来自服务端的消息");

        send(accept_connection,buffSend,BUFFER_LEN,0);

    }

    close(accept_connection);
    close(server_socket);
    printf("程序关闭");
    return 0;
}


#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h> //예네 작동안될수도
#include <time.h>
#include "client.h"

#define BUFSIZE 1024
#define SCHEDULE 3

using namespace std;

//전체적으로 문법
//특히 멤버변수 선언 주의

Client::Client(char* ip, int port, char* nickname) {
    int sock; //h에서 선언했기 때문에 초기화해야함
    int send_cnt; //h에서 선언했기 때문에 초기화해야함
    int valread; //h에서 선언했기 때문에 초기화해야함

    struct sockaddr_in serv_addr; //C++은 빼도됨
    struct timeval timeout ;
    fd_set reads, temps;
    char buffer[BUFSIZE] = {0};
    char my_nickname[BUFSIZE]; //초기화

    connect_to_server(ip, port); //리턴값을 받아야 함
    send_nickname(nickname);
    receive_all_nickname();
}

void Client::error_handling(const char* msg) {
    cerr << msg << endl;
    exit(1); //프로그램 도중에 끝나지 않도록 // return false로 타고 내려가서 안정적으로 종료하게
}

void Client::connect_to_server(char* ip, int port) { //리턴값 있어야함. 소켓값을 리턴해줘도 좋음
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) 
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(ip);
    serv_addr.sin_port=htons(port);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error!"); //에러핸들링하지않고 리턴 f/t하고 //여기서 exit호출하지 않도록

    FD_ZERO(&reads);
    FD_SET(0, &reads);
    FD_SET(sock, &reads);
}

void Client::send_nickname(char* nickname) { //리턴값을 넣어 제대로 실행됐는지 확인
    strcpy(my_nickname, nickname);
    send(sock, my_nickname, strlen(my_nickname), 0);
}

void Client::receive_all_nickname() {
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
}

void Client::handle_stdin() {
    valread = read(0, buffer, 1024);
    buffer[valread] = '\0';
    write(sock, buffer, strlen(buffer));
}

void Client::handle_server_input() {
    valread = read(sock, buffer, 1024);
    buffer[valread] = '\0';
    if(valread == 0) {
        FD_CLR(sock, &reads);
        close(sock);
        printf("closed server: %d", sock);
    } else {
        printf("%s", buffer);
    }
}

void Client::run() {
    while(true) {
        temps = reads;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
        buffer[0] = '\0';

        if(select(sock + 1, &temps, 0, 0, &timeout) == -1)
            error_handling("select() error") ;

        if(FD_ISSET(0, &temps)) handle_stdin();
        if(FD_ISSET(sock, &temps)) handle_server_input();
    }
}

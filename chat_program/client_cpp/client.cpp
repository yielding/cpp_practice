#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#define BUFSIZE 1024
#define SCHEDULE 3

using namespace std;

class Client {
private:
    int sock;
    int send_cnt;
    int valread;
    struct sockaddr_in serv_addr;
    struct timeval timeout ;
    fd_set reads, temps;
    char buffer[BUFSIZE] = {0};
    char my_nickname[1024];
    
    void error_handling(const char* msg) {
        cerr << msg << endl;
        exit(1);
    }
    
    void connect_to_server(char* ip, int port) {
        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == -1) error_handling("socket() error");

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr(ip);
        serv_addr.sin_port=htons(port);
        if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
            error_handling("connect() error!");

        FD_ZERO(&reads);
        FD_SET(0, &reads);
        FD_SET(sock, &reads);
    }
    
    void send_nickname(char* nickname) {
        strcpy(my_nickname, nickname);
        send(sock, my_nickname, strlen(my_nickname), 0);
    }
    
    void receive_all_nickname() {
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
    }
    
    void handle_stdin() {
        valread = read(0, buffer, 1024);
        buffer[valread] = '\0';
        write(sock, buffer, strlen(buffer));
    }
    
    void handle_server_input() {
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
    
public:
    Client(char* ip, int port, char* nickname) {
        connect_to_server(ip, port);
        send_nickname(nickname);
        receive_all_nickname();
    }
    
    void run() {
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
};
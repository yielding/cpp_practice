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

class Client {
private:
    int sock;
    int send_cnt;
    int valread; //변수 선언
    struct sockaddr_in serv_addr;
    struct timeval timeout ;
    fd_set reads, temps;
    char buffer[BUFSIZE];
    char my_nickname[BUFSIZE];
    
    void error_handling(const char* msg);
    
    void connect_to_server(char* ip, int port);
    
    void send_nickname(char* nickname);
    
    void receive_all_nickname();
    
    void handle_stdin();
    
    void handle_server_input();
    
public:
    Client(char* ip, int port, char* nickname);
    
    void run();
};
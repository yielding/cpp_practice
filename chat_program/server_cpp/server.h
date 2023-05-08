#include <vector>
#include <string>
#include <sys/select.h>
#define BUFSIZE 1024
using namespace std;

class Server {
    private:
        int serv_sock;
        struct sockaddr_in serv_addr;
        int str_len;
        struct timeval timeout;
        fd_set reads, temps;
        char nicknames[10][BUFSIZE]; //최대 10명의 닉네임 저장
        char all_nicknames[BUFSIZE*10];
        char send_message[BUFSIZE];
        int fd_max;

    public:
        Server(int port);

        void error_handling(const char* msg);

        void send_all(int fd, char* send_message);

        void connect_to_client(int serv_sock, fd_set reads, int& fd_max, char nicknames[][BUFSIZE], char* all_nicknames, char* message);
};
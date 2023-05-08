#include <vector>
#include <string>
#include <sys/select.h>
#define BUFSIZE 1024
using namespace std;

class Server {
    private:
        int serv_sock;
        void error_handling(const char* msg);

        void connect_to_client(int port);

        void send_all(int fd, char* send_message);

    public:
        char nicknames[10][BUFSIZE] = {""}; //최대 10명의 닉네임 저장
        char all_nicknames[BUFSIZE*10] = "현재 참여자 : ";
        char send_message[BUFSIZE] = "";
        fd_set reads, temps;
        int fd_max;

        Server(int port);

        void run();
};
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

int main(int argc, char *argv[]){
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <IP> <port> <nickname>" << endl;
        exit(1);
    }

    
    //Client client(argv[1], atoi(argv[2]), argv[3]);
    //client.run();

    return 0;
}

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>
#include "server.h"
#define BUFSIZE 1024
#define SCHEDULE 3

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 2) {
        cout << "Usage: " << argv[0] << "<port>" << endl;
        exit(1);
    }

    Server server(atoi(argv[1]));

    return 0;
}
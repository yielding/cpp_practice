#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "client.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4) 
    {
        cout << "Usage: " << argv[0] << " <IP> <port> <nickname>" << endl;
        return EXIT_FAILURE;
    }

    Client client(argv[1], atoi(argv[2]), string(argv[3])); //객체 만들고
    if (!client.connect_to_server(argv[1], atoi(argv[2])))
        return EXIT_FAILURE;

    if (!client.prepare_chat())
        return EXIT_FAILURE;

    client.print_friends_to(cout);
    client.run();

    return EXIT_SUCCESS;
}
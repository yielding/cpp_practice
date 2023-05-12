#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

using std::string;

using strings = std::vector<string>;

class Client 
{
public:
    Client(string const& ip, int port, string nickname);
    Client() = delete;
    Client(const Client&) = delete;
    ~Client();

public:
    auto connect_to_server(string const& ip, int port) -> bool;

    auto prepare_chat() -> bool;

    void run();

    void print_friends_to(std::ostream& out);

private:
    auto receive_all_nickname() -> strings;

    auto handle_stdin() -> bool;

    auto handle_server_input() -> bool;

private:
    int m_sock;
    bool m_connected;
    fd_set m_reads;
    string m_nickname;
    strings m_friends;

    string m_ip;
    int m_port;
};
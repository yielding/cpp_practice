#include <iostream>
#include <sstream>
#include <time.h>
#include <cstring>
#include <cstdint>

#include "client.h"

using namespace std;

const int BUF_SIZE = 1024 * 32;

////////////////////////////////////////////////////////////////////////////////
///
///
///
////////////////////////////////////////////////////////////////////////////////
namespace 
{
    auto split(string in, char delim) -> strings
    {
        strings result;
        stringstream ss(in);

        string item;
        while (getline(ss, item, delim))
            result.push_back(item);

        return result;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
///
///
////////////////////////////////////////////////////////////////////////////////
Client::Client(string const& ip, int port, string nickname) 
    : m_sock{-1}, m_ip{ip}, m_port{port}, m_nickname{nickname}
    , m_connected{false}
{
}

Client::~Client()
{
    close(m_sock);
}

bool Client::prepare_chat()
{
    if (!m_connected)
        return false;

    // 1. send my nickname
    auto ret = ::send(m_sock, m_nickname.data(), m_nickname.length(), 0);
    if (ret < 0)
        return false;

    m_friends = receive_all_nickname();

    return true;
}
auto Client::connect_to_server(string const& ip, int port) -> bool
{
    try
    {
        m_sock = socket(PF_INET, SOCK_STREAM, 0);
        if (m_sock == -1) 
        {
            // log("sock alloc error")
            return false;
        }

        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        serv_addr.sin_port = htons(port);

        if (connect(m_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        {
            // log("connect() error!");
            return false;
        }

        FD_ZERO(&m_reads);
        FD_SET(0, &m_reads);
        FD_SET(m_sock, &m_reads);
    }
    catch (...)
    {
        return false;
    }

    m_connected = true;

    return true;
}

// TODO read는 긴 데이터를 읽을 경우 나누어서 전송될 수 있으므로
// 1. 제일 먼저 size를 받고(4byte)
// 2. size 만큼 for loop를 돌면서 받는다.
auto Client::receive_all_nickname() -> strings
{
    strings result;

    char buf[4] = { 0 };
    if (read(m_sock, buf, 4) < 0)
        return result;

    auto to_read = ntohl(*(uint32_t *)buf); // endian 변환
    if (to_read <= 0)
        return result;

    char buffer[BUF_SIZE] = { 0 };
    int offset = 0;
    while (to_read > 0)
    {
        int rd = read(m_sock, buffer+offset, 1024);
        to_read -= rd;
        offset  += rd;
    }

    buffer[offset] = 0;

    string all_nicks(buffer, offset);

    return split(all_nicks, ';');
}

auto Client::handle_stdin() -> bool
{
    char buffer[BUF_SIZE] = { 0 };
    auto rd = read(0, buffer, 1024);
    if (rd < 0)
        return false;

    buffer[rd] = 0;

    return write(m_sock, buffer, strlen(buffer)) >= 0;
}

auto Client::handle_server_input() -> bool
{
    char buffer[BUF_SIZE] = { 0 };
    auto rd = read(m_sock, buffer, 1024);
    if (rd < 0)
        return false;

    buffer[rd] = '\0';
    if (rd == 0) 
    {
        FD_CLR(m_sock, &m_reads);
        close(m_sock);
        printf("closed server: %d", m_sock);
    } 
    else 
    {
        printf("%s", buffer);
    }

    return true;
}

void Client::run() 
{
    char buffer[BUF_SIZE] = { 0 };
    timeval timeout;

    while (true) 
    {
        auto temps = m_reads;
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;
        buffer[0] = '\0';

        if (select(m_sock + 1, &temps, 0, 0, &timeout) == -1)
        {
            // log("select() error") ;
            return;
        }

        if (FD_ISSET(0, &temps)) 
            handle_stdin();

        if (FD_ISSET(m_sock, &temps)) 
            handle_server_input();
    }
}


void Client::print_friends_to(ostream& out)
{
    for (auto& f: m_friends) 
        out << f << " ";

    out << endl;
}
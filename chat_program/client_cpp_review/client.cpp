// HOW를 하는 파일

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
namespace //annominus. 다른 cpp파일에서 사용할 수 없음 (=c의 static. 이 함수는 다른 파일에 영향을 주지 않는다.)
{ //server.cpp 파일에서는 쓰지 않기 위해.
    auto split(string in, char delim) -> strings //하나의 긴 문자열에서 delinm 기준으로 잘라 벡터에 담기
    {
        strings result;
        stringstream ss(in); //출력을 string에 담자

        string item;
        while (getline(ss, item, delim)) //string에서 라인을 읽어들여서
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
    : m_sock{-1}, m_ip{ip}, m_port{port}, m_nickname{nickname} //생성자 초기화를 중괄호로
    , m_connected{false} //전부 먼저 초기화를 하고 시작한다. -1은 덮어써지지만. 빌드와 릴리즈에서 랜덤한 값 넣어 이상해지지 않기 위해.
{ //여기안에서 생성자 초기화하는것보다 빠름! 이렇게 :,쓰면!
} 

Client::~Client() //소켓은 운영체제가 모두 사용하기 때문에, 다음에 또 쓰지 않기 위해 내가 할당한 메모리를 해제함
{
    close(m_sock);
}

bool Client::prepare_chat()
{
    if (!m_connected)
        return false; //리턴값 -1, 1등 주는게 좋음

    // 1. send my nickname
    auto ret = ::send(m_sock, m_nickname.data(), m_nickname.length(), 0);
    if (ret < 0)
        return false;

    m_friends = receive_all_nickname();

    return true;
}

auto Client::connect_to_server(string const& ip, int port) -> bool //얘는 됐냐안됐냐마ㅏㄴ 중요하기 때문에.
{
    try
    {
        m_sock = socket(PF_INET, SOCK_STREAM, 0); //
        if (m_sock == -1) 
        {
            // log("sock alloc error")
            return false;
        }

        sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr)); //한번 끝나면 없어지기 때문에.
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
    catch (...) //모든 예외 다 받음. 어떤예외라도 내가 처리하겠다.
    {
        return false;
    }

    m_connected = true;

    return true;
}

// TODO read는 긴 데이터를 읽을 경우 나누어서 전송될 수 있으므로 // 보낼때는 한번에 보내나, 읽을때는 잘라서 읽어야함.
// 1. 제일 먼저 size를 받고(4byte)
// 2. size 만큼 for loop를 돌면서 받는다.
auto Client::receive_all_nicknames() -> strings
{
    strings result;

    char buf[4] = { 0 }; //버퍼는 공유하는게 아니라 잠시쓰고버리는거니까 멤버변수에서 빼야함.
    if (read(m_sock, buf, 4) < 0) //TCP를 받을 때 나눠서 받아야함. 일단 4바이트만 받겠다.
        return result;

    //밑 세줄 일 하는 함수 get_packet_length라는 함수를 만들수도 있음
    auto to_read = ntohl(*(uint32_t *)buf); // endian 변환 =>1 integer로 변환. 총 읽어야 할 길이를 알게됨
    // ntohl: 네트워크 투 호스트 롱(타입), s는 short임
    if (to_read <= 0) //버퍼사이즈보다 작다고 가정하고
        return result;

    //밑 다섯개 일 하는 함수 pump_packet_to_string이라는 함수를 만들수도 있음
    //사용자에게 가까운 함수에 if와 while이 많으면 안도미
    char buffer[BUF_SIZE] = { 0 }; //읽을 버퍼 하나 잡음
    int offset = 0;
    while (to_read > 0) //ex) 16k를 읽어야한다면
    {
        int rd = read(m_sock, buffer+offset, to_read); //buffer+offset : buffer에 offset뒤에 값을 점점 써라.
        to_read -= rd; //to_read : 남은 양
        offset  += rd;
    }

    buffer[offset] = 0;

    string all_nicks(buffer, offset); //하나의 string

    return split(all_nicks, ';');
}

auto Client::handle_stdin() -> bool //갑자기 끊길수있기에
{
    char buffer[BUF_SIZE] = { 0 };
    auto rd = read(0, buffer, 1024);
    if (rd < 0)
        return false;

    buffer[rd] = 0;

    return write(m_sock, buffer, strlen(buffer)) >= 0; //성공 (0이상)만 리턴
}

auto Client::handle_server_input() -> bool
{
    char buffer[BUF_SIZE] = { 0 };
    auto rd = read(m_sock, buffer, BUF_SIZE);
    if (rd < 0)
        return false;

    buffer[rd] = '\0';
    if (rd == 0) 
    {
        FD_CLR(m_sock, &m_reads);
        close(m_sock);
        printf("closed server: %d", m_sock); //로그가 파일이나 네트워크로 넘어갈수도 있음.
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
            // log("select() error") ; //로그함수 만들고
            return;
        }

        if (FD_ISSET(0, &temps)) 
            handle_stdin();

        if (FD_ISSET(m_sock, &temps)) 
            handle_server_input();
    }
}

void Client::print_friends_to(ostream& out) //ostream이 부모, fout과 out은 자식.
//ifstreaam = istream. 이렇게 fstream을 이용하면 자유도가 굉장히 높아짐.
{
    for (auto& f: m_friends) //인자로 건네받은 객체를 넘겨줌
        out << f << " "; //f를 읽어서 socket으로 연결해줌.
        // 실제 회사에서는 소켓을 하나의 객체로 만듬.

    out << endl;
} //객체지향 관점에서 여러 파일
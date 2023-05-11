#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

using std::string; //네임스페이스 스탠다드 아닌 string만 따로 쓰겠다

using strings = std::vector<string>; //별칭 지정

class Client //이름을 ChatClient등 명확한 이름이 좋음.
{
public: //생셩자 소멸자
    Client(string const& ip, int port, string nickname);
    Client() = delete;
    Client(const Client&) = delete; //컴파일러 자동 생성 방지
    ~Client();

public: //대표 함수는 7개가 넘으면 쪼갤 수 있음
    auto connect_to_server(string const& ip, int port) -> bool;

    auto prepare_chat() -> bool;

    void run();

    void print_friends_to(std::ostream& out); //

private:
    auto receive_all_nickname() -> strings; //auto 쓴느 이유 : return 타입 앞에놔서 함수 시작 위치 달라지지 않게

    auto handle_stdin() -> bool;

    auto handle_server_input() -> bool;

private: //멤버 데이터를 다른 프로그램이 보지 못하게. 꼭 하고싶으면 get, set써야함.
// 클래스 안에서만 알아야하는. 1) 변하지않는. 2) 삭제되면안되는
    int m_sock;
    bool m_connected;
    fd_set m_reads;
    string m_nickname;
    strings m_friends;

    string m_ip;
    int m_port;
};
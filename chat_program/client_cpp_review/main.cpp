// WHAT을 하는 파일

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

#include "client.h"

using namespace std;

int main(int argc, char *argv[]) //메인에 가까운 pulbic 함수를 구현할때는 For, loop, if (how)가 들어가면 안됨.
// if 중첩되면 안됨. 하나씩 쪼개야 함.
{
    if (argc != 4) 
    {
        cout << "Usage: " << argv[0] << " <IP> <port> <nickname>" << endl;
        return EXIT_FAILURE;
    }

    Client client(argv[1], atoi(argv[2]), string(argv[3])); //객체 만들고
    if (!client.connect_to_server(argv[1], atoi(argv[2])))
        return EXIT_FAILURE; //0 (스탠다드에서 정의됨. 쉘에게 알려줌) == return 1

    if (!client.prepare_chat()) //여기서 친구목록 받아옴
        return EXIT_FAILURE;

    client.print_friends_to(cout); //cout은 outstream 객체를 상속받음. 그래서 파일이나 소켓 등 전부 가능. fstring은 파일
    // 어떤 일을 하도록 
    client.run();

    return EXIT_SUCCESS; //0 (쉘 스크립트에서 이어서 실행하도록 리턴값 주는거. 유닉스는 여러 프로그램 돌릴 수 있다.)
    // 프로그램별로 성공 실패 알수있음. 통용적으로 성공 0, 실패 1, 에러별 리턴값 다 다르게 만들 수 있음.
}
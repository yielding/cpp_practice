#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdint>

#include "client.h"

using namespace std;

const int BUF_SIZE = 1024 * 32;

// client의 private 함수 구현
auto Client::pump_packet_to_string(int m_sock, int to_read) -> std::string
{
    char buffer[BUF_SIZE] = { 0 };
    int offset = 0;
    while (to_read > 0)
    {
        int rd = read(m_sock, buffer+offset, 1024);
        to_read -= rd;
        offset  += rd;
    }
    return buffer;
}

// 함수의 test case 먼저 정의
TEST(pump_packet_to_string) {
    m_sock = socket(PF_INET, SOCK_STREAM, 0);
    auto to_read = ntohl(*(uint32_t *)buf); // endian 변환
    pump_packet_to_string(m_sock, 0)
    ASSERT_EQ(offset, 5)

    EXPECT_FALSE(pump_packet_to_string(m_sock, 0));
    EXPECT_FALSE(pump_packet_to_string(m_sock, -1));
};

// 테스트
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
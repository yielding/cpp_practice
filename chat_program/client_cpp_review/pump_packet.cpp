#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdint>

#include "client.h"

using namespace std;

const int BUF_SIZE = 1024 * 32;

// client의 private 함수 구현
auto Client::pump_packet_to_string(int m_sock, int to_read) -> bool
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

/* google test?
// 함수의 test case 먼저 정의
TEST(pump_packet_to_string) {
    EXPECT_FALSE(pump_packet_to_string(&m_sock, 0));
    EXPECT_FALSE(pump_packet_to_string(&m_sock, -1));
};

TEST(pump_packet_to_string) {
    EXPECT_FALSE(pump_packet_to_string(NULL, to_read));
};

TEST(pump_packet_to_string) {
    EXPECT_TRUE(pump_packet_to_string(&m_sock, 1024));
    EXPECT_TRUE(pump_packet_to_string(&m_sock, 16));
};
 */

// 테스트
int main(int argc, char **argv) {
    assert(pump_packet_to_string(&m_sock, 0));
}
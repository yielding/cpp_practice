#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <gtest/gtest.h>

#include "client.h"

using namespace std;

const int BUF_SIZE = 1024 * 32;

// client의 private 함수 구현
auto Client::pump_packet_to_string(auto m_sock, auto to_read) -> bool
{
    char buffer[BUF_SIZE] = { 0 };
    int offset = 0;
    while (to_read > 0)
    {
        int rd = read(m_sock, buffer+offset, 1024);
        to_read -= rd;
        offset  += rd;
    }
}

// 함수의 test case 먼저 정의
TEST(Pump_Packet_to_Striing_Test, NegativeToread) {
    EXPECT_TRUE(pump_packet_to_string(2));
    EXPECT_FALSE(pump_packet_to_string(3));
}

TEST(Pump_Packet_to_Striing_Test, EmptyMsocket) {
    EXPECT_TRUE(pump_packet_to_string(2));
    EXPECT_FALSE(pump_packet_to_string(3));
}

TEST(Pump_Packet_to_Striing_Test, Positive) {
    EXPECT_TRUE(pump_packet_to_string(2));
    EXPECT_FALSE(pump_packet_to_string(3));
}

// 테스트
int main(int argc, char **argv) {
    testing::pump_packet_to_string(&m_sock, to_read);
    return RUN_ALL_TESTS();
}
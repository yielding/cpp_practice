#include <gtest/gtest.h>
#include <string>
#include <cstring>

using namespace std;

// buffer: 저장된 문자열을 가리키는 포인터
// offset: buffer 내에서 현재 위치를 나타내는 인덱스
// length: buffer의 총 길이
// read_size: 읽어와야 할 문자열의 길이
void read_string(char* buffer, size_t& offset, size_t length, size_t read_size, std::string& result) {
    if (offset >= length) {
        return;
    }
    
    // 읽어와야 할 문자열의 길이가 남은 버퍼의 길이보다 큰 경우
    if (read_size > length - offset) {
        read_size = length - offset;
    }
    
    // 현재 위치(offset)에서 read_size만큼의 문자열을 읽어서 result에 저장
    result.assign(buffer + offset, read_size);
    
    // offset을 읽어온 문자열 길이만큼 이동
    offset += read_size;
}

// Test Fixture
class ReadStringTest : public ::testing::Test {
protected:
    void SetUp() override {
        buffer = new char[14];
        strcpy(buffer, "Hello, World!");
        offset = 0;
        length = 13;
        read_size = 5;
    }
    void TearDown() override {
        delete[] buffer;
    }
    char* buffer;
    size_t offset;
    size_t length;
    size_t read_size;
    string result;
};

// Test Case
TEST_F(ReadStringTest, Test1) {
    read_string(buffer, offset, length, read_size, result);
    ASSERT_EQ(offset, 5);
    ASSERT_EQ(result, "Hello");
}

TEST_F(ReadStringTest, Test2) {
    read_string(buffer, offset, length, read_size, result);
    ASSERT_EQ(offset, 10);
    ASSERT_EQ(result, ", Wor");
}

TEST_F(ReadStringTest, Test3) {
    read_string(buffer, offset, length, read_size, result);
    ASSERT_EQ(offset, 13);
    ASSERT_EQ(result, "ld!");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
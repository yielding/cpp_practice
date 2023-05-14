#include <string>
#include <cstring>
#include <iostream>

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

// 테스트
int main() {
    char buffer[] = "Hello, World!";
    size_t offset = 0;
    size_t length = 13;
    size_t read_size = 5;
    string result;
    
    read_string(buffer, offset, length, read_size, result);
    
    // 결과 출력
    cout << "Offset: " << offset << endl;
    cout << "Result: " << result << endl;
    
    return 0;
}
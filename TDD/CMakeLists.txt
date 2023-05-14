cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 버전 지정
project(MyProject VERSION 1.0)

# googletest 라이브러리 가져오기
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# 테스트 타겟 추가
add_executable(read_string_tdd_gtest read_string_tdd_gtest.cpp)
target_link_libraries(read_string_tdd_gtest gtest_main)
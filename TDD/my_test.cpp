// my_test.cpp

#include <gtest/gtest.h>

TEST(MyTest, FirstTest) {
    EXPECT_EQ(2+2, 4);
}

TEST(MyTest, SecondTest) {
    EXPECT_TRUE(true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
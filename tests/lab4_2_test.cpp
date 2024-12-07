#include <gtest/gtest.h>
#include <functions.h>

TEST(Library2, TestSquare1) {
    float result = Square(1.5, 3.0);
    EXPECT_FLOAT_EQ(result, 2.25);
}

TEST(Library2, TestSquare2) {
    float result = Square(4.0, 3.0);
    EXPECT_FLOAT_EQ(result, 6.0);
}

TEST(Library2, TestPrimeCount1) {
    int result = PrimeCount(1, 5);
    EXPECT_EQ(result, 3);
}

TEST(Library2, TestPrimeCount2) {
    int result = PrimeCount(2, 11);
    EXPECT_EQ(result, 5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>
#include <functions.h>

TEST(Library1, TestSquare1) {
    float result = Square(1.5, 3.0);
    EXPECT_FLOAT_EQ(result, 4.5);
}

TEST(Library1, TestSquare2) {
    float result = Square(4.0, 3.0);
    EXPECT_FLOAT_EQ(result, 12.0);
}

TEST(Library1, TestPrimeCount1) {
    int result = PrimeCount(1, 5);
    EXPECT_EQ(result, 3);
}

TEST(Library1, TestPrimeCount2) {
    int result = PrimeCount(2, 11);
    EXPECT_EQ(result, 5);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
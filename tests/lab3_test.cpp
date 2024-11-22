#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "parent.hpp"

TEST(ParentRoutineTest, CalculatesSumCorrectly) {
    std::ostringstream outputStream;
    std::string fileName = getenv("PATH_TO_TEST_FILE");
    
    //Cодержимое test.txt:
    //100 10 50
    //40 -10 10

    int expectedOutput = 200;

    ParentRoutine(fileName, outputStream);

    std::string output = outputStream.str();
    std::istringstream iss(output);
    int realOutput = 0;
    iss >> realOutput;

    EXPECT_EQ(realOutput, expectedOutput);
}

TEST(ParentRoutineTest, EmptyFile) {
    std::ostringstream outputStream;
    std::string fileName = getenv("PATH_TO_EMPTY_TEST_FILE");
    
    int expectedOutput = 0;

    ParentRoutine(fileName, outputStream);

    std::string output = outputStream.str();
    std::istringstream iss(output);
    int realOutput = 0;
    iss >> realOutput;

    EXPECT_EQ(realOutput, expectedOutput);
}

TEST(ParentRoutineTest, CalculatesSumCorrectly2) {
    std::ostringstream outputStream;
    std::string fileName = getenv("PATH_TO_TEST_FILE2");
    
    //Cодержимое test2.txt:
    //100 10 50
    //40 -10 10 100000 0

    int expectedOutput = 100200;

    ParentRoutine(fileName, outputStream);

    std::string output = outputStream.str();
    std::istringstream iss(output);
    int realOutput = 0;
    iss >> realOutput;

    EXPECT_EQ(realOutput, expectedOutput);
    
}
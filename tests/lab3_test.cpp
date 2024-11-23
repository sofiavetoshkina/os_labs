#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "parent.hpp"

TEST(ParentRoutineTest, CalculatesSumCorrectly) {
    std::ostringstream outputStream;
    const char* fileName = getenv("PATH_TO_TEST_FILE");
    if (fileName == nullptr) {
            perror("Переменная PATH_TO_TEST_FILE не установлена");
            exit(1);
    }
    
    //Cодержимое test.txt:
    //100 10 50
    //40 -10 10

    const int expectedOutput = 200;

    ParentRoutine(fileName, outputStream);

    std::string output = outputStream.str();
    std::istringstream iss(output);
    int realOutput = 0;
    iss >> realOutput;

    EXPECT_EQ(realOutput, expectedOutput);
}

TEST(ParentRoutineTest, EmptyFile) {
    std::ostringstream outputStream;
    const char* fileName = getenv("PATH_TO_EMPTY_TEST_FILE");
    if (fileName == nullptr) {
            perror("Переменная PATH_TO_EMPTY_TEST_FILE не установлена");
            exit(1);
    }
    
    const int expectedOutput = 0;

    ParentRoutine(fileName, outputStream);

    std::string output = outputStream.str();
    std::istringstream iss(output);
    int realOutput = 0;
    iss >> realOutput;

    EXPECT_EQ(realOutput, expectedOutput);
}

TEST(ParentRoutineTest, CalculatesSumCorrectly2) {
    std::ostringstream outputStream;
    const char* fileName = getenv("PATH_TO_TEST_FILE2");
    if (fileName == nullptr) {
            perror("Переменная PATH_TO_TEST_FILE2 не установлена");
            exit(1);
    }
    
    //Cодержимое test2.txt:
    //100 10 50
    //40 -10 10 100000 0

    const int expectedOutput = 100200;

    ParentRoutine(fileName, outputStream);

    std::string output = outputStream.str();
    std::istringstream iss(output);
    int realOutput = 0;
    iss >> realOutput;

    EXPECT_EQ(realOutput, expectedOutput);
}
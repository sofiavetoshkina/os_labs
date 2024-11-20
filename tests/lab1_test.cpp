#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "parent.hpp"

TEST(Lab1Test, CorrectSum) {

    std::ostringstream outputStream;
    std::string fileName = getenv("PATH_TO_TEST_FILE");
    
    //Cодержимое text.txt:
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
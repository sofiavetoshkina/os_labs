#include <gtest/gtest.h>
#include <string>

#include <parent.hpp>


TEST(Lab1Test, CorrectSum) {
    // Сохраняем старый буфер стандартного вывода
    std::streambuf* oldCoutBuf = std::cout.rdbuf();

    // Создаем строковый поток для захвата вывода
    std::ostringstream outputStream;
    std::cout.rdbuf(outputStream.rdbuf());

    std::string fileName = "test.txt"; // Tест вызывается из той папки, где находится test.txt
    
    //Cодержимое text.txt:
    //100 10 50
    //40 -10 10

    int expectedOutput = 200;

    // Выполняем функцию
    ParentRoutine(fileName);

    // Восстанавливаем старый буфер
    std::cout.rdbuf(oldCoutBuf);

    // Получаем захваченный вывод как строку
    std::string output = outputStream.str();

    std::istringstream iss(output);
    int number = 0;
    iss >> number;

    // Проверяем, совпадает ли вывод с ожидаемым результатом
    EXPECT_EQ(number, expectedOutput);
}
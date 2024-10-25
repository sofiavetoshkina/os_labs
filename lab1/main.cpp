#include <iostream>
#include <string>
#include <parent.hpp>

int main() {
    std:: string fileName;
    std::cout << "Введите название файла, где необходимо посчитать сумму чисел: ";
    std::cin >> fileName;

    ParentRoutine(fileName);

    return 0;
}
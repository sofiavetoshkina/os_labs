#include <iostream>
#include <string>
#include <csignal>
#include "parent.hpp"

int main() {
    std::string fileName;
    std::cout << "Введите название файла, где необходимо посчитать сумму чисел: ";
    std::cin >> fileName;

    try {
        ParentRoutine(fileName, std::cout);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

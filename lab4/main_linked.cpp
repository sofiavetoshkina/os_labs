#include <iostream>
#include "functions.h"

int main() {
    std::cout << "Статическая линковка\n";

    while (true) {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "1 A B - Вычислить площадь (прямоугольник или треугольник)\n";
        std::cout << "2 A B - Вычислить количество простых чисел в диапазоне [A,B]\n";
        std::cout << "3 - Выход";
        std::cout << "\n---------------------------------------------------------------\n";
        
        int command;
        std::cin >> command;

        if (command == 1) {
            float A, B;
            std::cin >> A >> B;
            float result = Square(A, B);
            std::cout << "Площадь: " << result << "\n";
        } else if (command == 2) {
            int A, B;
            std::cin >> A >> B;
            int result = PrimeCount(A, B);
            std::cout << "Количество простых чисел в диапазоне [A,B]: " << result << "\n";
        } else if (command == 3) {
            break;
        } else {
            std::cout << "Такого пункта меню не существует\n";
        }
    }

    return 0;
}
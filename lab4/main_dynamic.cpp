#include <iostream>
#include <dlfcn.h>
#include <cstring>

using SquareFunc = float(*)(float, float);
using PrimeCountFunc = int(*)(int, int);

int main() {
    std::cout << "Динамическая загрузка библиотек\n";

    const char* pathToLib1 = std::getenv("PATH_TO_LIBRARY1");
    if (!pathToLib1) {
        std::cerr << "Переменная PATH_TO_LIBRARY1 не задана" << std::endl;
        return 1;
    }

    const char* pathToLib2 = std::getenv("PATH_TO_LIBRARY2");
    if (!pathToLib2) {
        std::cerr << "Переменная PATH_TO_LIBRARY2 не задана" << std::endl;
        return 1;
    }

    const char* lib_paths[] = {pathToLib1, pathToLib2};

    int current_lib = 0;

    std::cout << "\nКакую библиотеку вы хотели бы загрузить?\n";
    std::cout << "1 - implementation1.so (Площадь прямоугольника, Наивный поиск простых чисел)\n";
    std::cout << "2 - implementation2.so (Площадь прямоугольного треугольника, Поиск простых чисел при помощи алгоритма решето Эратосфена)\n";

    std::cin >> current_lib;

    if (current_lib != 1 && current_lib != 2) {
        std::cout << "Неверная команда\n";
        return 1;
    }

    current_lib--;

    void* handle = dlopen(lib_paths[current_lib], RTLD_LAZY);

    if (!handle) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << "\n";
        return 1;
    }

    SquareFunc Square = reinterpret_cast<SquareFunc>(dlsym(handle, "Square"));
    PrimeCountFunc PrimeCount = reinterpret_cast<PrimeCountFunc>(dlsym(handle, "PrimeCount"));

    char* error;
    if ((error = dlerror()) != nullptr) {
        std::cerr << "Ошибка получения символа: " << error << "\n";
        dlclose(handle);
        return 1;
    }

    while (true) {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "Библиотека: " << lib_paths[current_lib] << "\n";
        std::cout << "0 - Следующая библиотека\n";
        std::cout << "1 A B - Вычислить площадь (прямоугольник или треугольник)\n";
        std::cout << "2 A B - Вычислить количество простых чисел в диапазоне [A,B]\n";
        std::cout << "3 - Выход";
        std::cout << "\n---------------------------------------------------------------\n";
        
        int command;
        std::cin >> command;

        if (command == 0) {
            dlclose(handle);
            current_lib = 1 - current_lib;
            handle = dlopen(lib_paths[current_lib], RTLD_LAZY);
            if (!handle) {
                std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << "\n";
                return 1;
            }
            Square = reinterpret_cast<SquareFunc>(dlsym(handle, "Square"));
            PrimeCount = reinterpret_cast<PrimeCountFunc>(dlsym(handle, "PrimeCount"));
        } else if (command == 1) {
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
            std::cout << "Неверная команда\n";
        }
    }

    dlclose(handle);
    return 0;
}
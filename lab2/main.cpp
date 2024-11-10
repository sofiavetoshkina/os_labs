#include <iostream>
#include "filter.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Введите: " << argv[0] << " <count_threads>\n";
        return 1;
    }

    int countThreads = std::stoi(argv[1]);

    int rows, cols, kernelSize, iterations;
    std::cout << "Введите <rows> <cols> <kernelSize> <count_of_filter>: " << std::endl;
    std::cin >> rows >> cols >> kernelSize >> iterations;

    TMatrix matrix(rows, std::vector<double>(cols));
    TMatrix kernel(kernelSize, std::vector<double>(kernelSize));

    std::cout << "Введите вещественную матрицу: " << std::endl;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            std::cin >> matrix[i][j];

    std::cout << "Введите матрицу свертки(нормированность соблюдается): " << std::endl;
    for (int i = 0; i < kernelSize; ++i)
        for (int j = 0; j < kernelSize; ++j)
            std::cin >> kernel[i][j];

    TMatrix result = ApplyConvolution(matrix, kernel, iterations, countThreads);

    std::cout << "Результирующая матрица после наложения фильтра K раз: " << std::endl;
    for (const auto& row : result) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << '\n';
    }

    return 0;
}

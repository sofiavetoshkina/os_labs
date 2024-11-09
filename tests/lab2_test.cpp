#include <chrono>
#include <gtest/gtest.h>
#include <vector>
#include "filter.h"

TEST(ConvolutionTest, SingleThreadCorrectness) {

    TMatrix matrix = {
        {10.0, 10.0, 10.0},
        {10.0, 10.0, 10.0},
        {10.0, 10.0, 10.0}
    };

    // Принимаем, что ядро нормированно(сумма всех элементов = 1)
    TMatrix kernel = {
        {0.1, 0.1, 0.1},
        {0.1, 0.2, 0.1},
        {0.1, 0.1, 0.1}
    };

    TMatrix expected = {
        {5.0, 7.0, 5.0},
        {7.0, 10.0, 7.0},
        {5.0, 7.0, 5.0}
    };

    int countThreads = 1;
    int iterations = 1;

    TMatrix result = ApplyConvolution(matrix, kernel, iterations, countThreads);
    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[0].size(); j++) {
            EXPECT_DOUBLE_EQ(expected[i][j], result[i][j]);
        }
    }
}

TEST(ConvolutionTest, SingleThreadCorrectness2) {

    TMatrix matrix = {
        {10.0, 10.0, 10.0},
        {10.0, 10.0, 10.0},
        {10.0, 10.0, 10.0}
    };

    // Принимаем, что ядро нормированно(сумма всех элементов = 1)
    TMatrix kernel = {
        {0.1, 0.1, 0.1},
        {0.1, 0.2, 0.1},
        {0.1, 0.1, 0.1}
    };

    TMatrix expected = {
        {3.4, 4.8, 3.4},
        {4.8, 6.8, 4.8},
        {3.4, 4.8, 3.4}
    };

    int countThreads = 1;
    int iterations = 2;

    TMatrix result = ApplyConvolution(matrix, kernel, iterations, countThreads);
    for(size_t i = 0; i < matrix.size(); i++) {
        for(size_t j = 0; j < matrix[0].size(); j++) {
            EXPECT_DOUBLE_EQ(expected[i][j], result[i][j]);
        }
    }
}

TEST(ConvolutionTest, MultiThreadIsSameAsSingleThread) {

    TMatrix largeMatrix(100, std::vector<double>(100, 1.0));

    TMatrix kernel = {
        {0.0, -1.0, 0.0},
        {-1.0, 5.0, -1.0},
        {0.0, -1.0, 0.0}
    };
    
    int iterations = 1;

    TMatrix result = ApplyConvolution(largeMatrix, kernel, iterations, 1);
    TMatrix resultMulti = ApplyConvolution(largeMatrix, kernel, iterations, 4);

    for(size_t i = 0; i < largeMatrix.size(); i++) {
        for(size_t j = 0; j < largeMatrix[0].size(); j++) {
            EXPECT_DOUBLE_EQ(result[i][j], resultMulti[i][j]);
        }
    }
}

TEST(ConvolutionTest, MultiThreadIsFasterThanSingleThread) {

    TMatrix largeMatrix(1000, std::vector<double>(1000, 0.1));

    TMatrix kernel = {
        {0.1, 0.1, 0.1},
        {0.1, 0.2, 0.1},
        {0.1, 0.1, 0.1}
    };
    
    auto startSingle = std::chrono::high_resolution_clock::now();
    ApplyConvolution(largeMatrix, kernel, 1, 1);  // Однопоточная версия
    auto endSingle = std::chrono::high_resolution_clock::now();
    auto singleThreadTime = std::chrono::duration_cast<std::chrono::milliseconds>(endSingle - startSingle).count();

    auto startMulti = std::chrono::high_resolution_clock::now();
    ApplyConvolution(largeMatrix, kernel, 1, 8);  // Многопоточная версия
    auto endMulti = std::chrono::high_resolution_clock::now();
    auto multiThreadTime = std::chrono::duration_cast<std::chrono::milliseconds>(endMulti - startMulti).count();

    ASSERT_GT(singleThreadTime, multiThreadTime);
}

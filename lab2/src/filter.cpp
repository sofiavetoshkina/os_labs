#include <algorithm>
#include <iostream>
#include <pthread.h>
#include <vector> 
#include "filter.h"

struct threadArgs {
    const TMatrix* matrix;
    const TMatrix* kernel;
    TMatrix* resultMatrix;
    int startRow;
    int endRow;
    int kernelSize;
};

void* ApplyConvolutionToRows(void* args) {
    
    threadArgs* params = (threadArgs*)(args);
    const TMatrix& matrix = *params->matrix;
    const TMatrix& kernel = *params->kernel;
    TMatrix& resultMatrix = *params->resultMatrix;
    int kernelSize = params->kernelSize;
    int halfKernel = kernelSize / 2;

    for (int i = params->startRow; i < params->endRow; ++i) {
        for (int j = 0; j < (int)matrix[0].size(); ++j) {
            double value = 0.0;
            for (int ki = 0; ki < kernelSize; ++ki) {
                for (int kj = 0; kj < kernelSize; ++kj) {
                    int ni = i + ki - halfKernel;
                    int nj = j + kj - halfKernel;

                    if (ni >= 0 && ni < (int)matrix.size() && nj >= 0 && nj < (int)matrix[0].size()) {
                        value += matrix[ni][nj] * kernel[ki][kj];
                    }
                }
            }

            resultMatrix[i][j] = value;
        }
    }

    return nullptr;
}

TMatrix ApplyConvolution(const TMatrix& matrix, const TMatrix& kernel, int iterations, int countThreads) {
    
    int rows = matrix.size();
    int cols = matrix[0].size();
    int kernelSize = kernel.size();
    TMatrix current = matrix;
    TMatrix next(rows, std::vector<double>(cols));

    for (int iter = 0; iter < iterations; ++iter) {
        std::vector<pthread_t> threads(countThreads);
        std::vector<threadArgs> args(countThreads);
        
        int baseRowsPerThread = rows / countThreads;
        int extraRows = rows % countThreads;

        int currentRow = 0;
        for (int t = 0; t < countThreads; ++t) {
            int threadRows = baseRowsPerThread + (t < extraRows ? 1 : 0);

            args[t] = {
                &current,
                &kernel,
                &next,
                currentRow,
                currentRow + threadRows,
                kernelSize
            };

            pthread_create(&threads[t], nullptr, ApplyConvolutionToRows, &args[t]);
            currentRow += threadRows;
        }

        for (int t = 0; t < countThreads; ++t) {
            pthread_join(threads[t], nullptr);
        }

        std::swap(current, next);
    }

    return current;
}
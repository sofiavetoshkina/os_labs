#pragma once

#include <vector>

using TMatrix = std::vector<std::vector<double>>;

TMatrix ApplyConvolution(const TMatrix& matrix, const TMatrix& kernel, int iterations, int countThreads);
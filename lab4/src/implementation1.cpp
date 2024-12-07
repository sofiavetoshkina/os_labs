#include <cmath>
#include "functions.h"

extern "C" {
    float Square(float A, float B) {
        return A * B;
    }

    int PrimeCount(int A, int B) {
        int count = 0;
        for (int num = A; num <= B; ++num) {
            if (num < 2) continue;
            bool is_prime = true;
            for (int div = 2; div <= std::sqrt(num); ++div) {
                if (num % div == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) ++count;
        }
        return count;
    }
}
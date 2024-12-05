#include <cmath>
#include <vector>
#include "functions.h"

extern "C" {
    float Square(float A, float B) {
        return 0.5f * A * B;
    }

    int PrimeCount(int A, int B) {
        if (B < 2) return 0;

        std::vector<bool> is_prime(B + 1, true);
        is_prime[0] = is_prime[1] = false;

        for (int p = 2; p <= std::sqrt(B); ++p) {
            if (is_prime[p]) {
                for (int multiple = p * p; multiple <= B; multiple += p) {
                    is_prime[multiple] = false;
                }
            }
        }

        int count = 0;
        for (int i = A; i <= B; ++i) {
            if (is_prime[i]) ++count;
        }
        return count;
    }
}
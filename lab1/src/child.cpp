#include <iostream>

int main() {
    int number {0};
    long long sum {0};
    while (std::cin >> number) {
        sum += number;
    }
    std::cout << sum << std::endl;

    return 0;
}


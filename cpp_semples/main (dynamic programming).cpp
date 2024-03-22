#include <limits>
#include <iostream>
#include <stdint.h>
#include <algorithm>

// prises are indexed for (length - 1): prises[i] corresponds to i + 1 length
template <typename T>
T rodCuttingProblem(const std::vector<T>& prises, size_t length) {
    std::vector<T> revenues;
    revenues.push_back(0);

    for (size_t i = 0; i < length; ++i) {
        T currRev = std::numeric_limits<T>::min();

        for (size_t j = 0; j <= i; ++j) {
            currRev = std::max(currRev, static_cast<T>(prises[j] + revenues[i - j]));
        }

        revenues.push_back(currRev);
    }

    return revenues[length];
}

int main() {
    {
        std::vector<uint8_t> prises {1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
        auto revenue = rodCuttingProblem(prises, prises.size());
        std::cout << "revenue: " << static_cast<int>(revenue) << std::endl;
    }
}

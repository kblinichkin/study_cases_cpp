#include <algorithm>
#include <random>
#include <chrono>
#include <array>
#include <iostream>
// #include "math.h"

void checkShuffle() {
    std::array<int, 10> foo1 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::array<int, 10> foo2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    auto rng = std::default_random_engine {seed};
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(std::begin(foo1), std::end(foo1), rng);
    std::shuffle(std::begin(foo2), std::end(foo2), g);

    std::cout << "Shuffling with std::default_random_engine" << std::endl;
    for (const auto& elm : foo1)
        std::cout << elm << ", ";
    std::cout << std::endl;

    std::cout << "Shuffling with std::mt19937" << std::endl;
    for (const auto& elm : foo2)
        std::cout << elm << ", ";
    std::cout << std::endl;
}

template <typename T>
int interview(T&& candidate) {
    // TODO: check if T is numeric
    return candidate;
}

template <typename T>
void hire(T id) {
    std::cout << "Hired candidate: " << id << std::endl;
}

// C must be a standard container
template <typename C>
void hireAssistant(const C& c) {
    int bestResult = 0;
    for (auto it = std::begin(c); it != std::end(c); ++it) {
        auto result = interview(*it);
        if (result > bestResult) {
            bestResult = result;
            // FIXME: not the best performance for the random_access iterators
            hire(std::distance(std::begin(c), it));
        }
    }
}

int calculateBirthdayParadax_1(long long n) {
    return 1 + sqrt(1 + (8 * log(2)) * n) / 2;
}

int calculateBirthdayParadax_2(long long n) {
    return sqrt(2 * n) + 1;
}

int calculateBirthdayParadax_3(long long n) {
    return sqrt(n);
}

int main() {
    // checkShuffle();
    //---------------------------------

    // std::array<int, 10> candidates {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    //
    // std::cout << "The worst input" << std::endl;
    // hireAssistant(candidates);
    //
    // std::random_device rd;
    // std::mt19937 g(rd());
    // std::shuffle(std::begin(candidates), std::end(candidates), g);
    //
    // std::cout << "The permutated input" << std::endl;
    // hireAssistant(candidates);
    //
    // // theoretical complexity ln(10) = 2.303
    // // permutated output (run 13 times) = 2.846
    //----------------------------------

    // constexpr long long n = 100'000'000'000'000;
    // std::cout << "1st formula (" << n <<"): " << calculateBirthdayParadax_1(n)
    //           << "; err=" << calculateBirthdayParadax_1(n) / static_cast<float>(calculateBirthdayParadax_3(n)) << std::endl;
    // std::cout << "2nd formula (" << n <<"): " << calculateBirthdayParadax_2(n)
    //           << "; err=" << calculateBirthdayParadax_2(n) / static_cast<float>(calculateBirthdayParadax_3(n)) << std::endl;
    // std::cout << "3rd formula (" << n <<"): " << calculateBirthdayParadax_3(n) << std::endl;

    {
        std::random_device rd;
        std::mt19937 g(rd());

        {
            // probability of 5 to be chosen out of the 6 cube through 5 tosses
            const int n = 1'000'000;
            int counter {0};
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < 5; j++) {
                    int val = g() % 6 + 1;
                    if (val == 5)
                        ++counter;
                }
            }
            std::cout << "p1: " << counter / static_cast<double>(n) * 100 << "%" << std::endl;
        }

        {
            // probability of 5 to be chosen out of the 6 cube on the 5th toss and that 4 tosses
            // before are not 5
            const int n = 1'000'000;
            int counter {0};
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < 5; j++) {
                    int val = g() % 6 + 1;
                    if (j == 4 && val == 5)
                        ++counter;
                    else if (val == 5)
                        break;
                }
            }
            std::cout << "p2: " << counter / static_cast<double>(n) * 100 << "%" << std::endl;
        }

        {
            // Probability of 5 to be countered once out of 5 tosses
            const int n = 1'000'000;
            int counter {0};
            for (int i = 0; i < n; ++i) {
                int c {0};
                for (int j = 0; j < 5; j++) {
                    int val = g() % 6 + 1;
                    if (val == 5)
                        ++c;
                }
                if (c == 1)
                    ++counter;
            }
            std::cout << "p3: " << counter / static_cast<double>(n) * 100 << "%" << std::endl;
        }

        {
            // Probability of 5 not to be encountered out of 5 tosses
            const int n = 1'000'000;
            int counter {0};
            for (int i = 0; i < n; ++i) {
                int c {0};
                for (int j = 0; j < 5; j++) {
                    int val = g() % 6 + 1;
                    if (val != 5)
                        ++c;
                }
                if (c == 5)
                    ++counter;
            }
            std::cout << "p4: " << counter / static_cast<double>(n) * 100 << "%" << std::endl;
        }

        {
            //
            const int n = 1'000'000;
            int counter {0};

            for (int i = 0; i < n; ++i) {
                std::array<int, 6> arr{0};

                bool stop = false;
                do {
                    ++counter;
                    int val = g() % 6;
                    ++arr[val];

                    stop = true;
                    for (const auto& v : arr) {
                        if (v == 0) {
                            stop = false;
                            break;
                        }
                    }
                } while (!stop);
            }
            std::cout << "p4: " << counter / static_cast<double>(n) << " tosses" << std::endl;
        }
    }
}

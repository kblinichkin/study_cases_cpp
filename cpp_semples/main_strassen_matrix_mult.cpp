#include <iostream>
#include <iomanip>
#include <chrono>
#include <cassert>

#include <vector>
#include <math.h>
#include <future>

template<class T>
class SquareMatrix {
    std::vector<T> m_mem;
    size_t m_size;
public:
    SquareMatrix(size_t size) : m_size(size) { m_mem.resize(size * size); }
    SquareMatrix(std::initializer_list<T> list) : m_mem(list), m_size(sqrt(m_mem.size())) {}

    SquareMatrix(SquareMatrix&&) = default;
    SquareMatrix& operator=(SquareMatrix&&) = default;
    SquareMatrix(const SquareMatrix&) = default;
    SquareMatrix& operator=(const SquareMatrix&) = default;

    size_t size() const { return m_size; }
    T& at(size_t i, size_t j) { return m_mem[m_size * i + j]; }
    const T& at(size_t i, size_t j) const { return m_mem[m_size * i + j]; }
};

template <class T>
SquareMatrix<T> operator+(const SquareMatrix<T>& a, const SquareMatrix<T>& b) {
    auto size = a.size();
    assert(size == b.size());

    SquareMatrix res(a);
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            res.at(i, j) += b.at(i, j);

    return res;
}

template <class T>
SquareMatrix<T> operator-(const SquareMatrix<T>& a, const SquareMatrix<T>& b) {
    auto size = a.size();
    assert(size == b.size());

    SquareMatrix res(a);
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            res.at(i, j) -= b.at(i, j);

    return res;
}

template <class T>
SquareMatrix<T> strassen(const SquareMatrix<T>& matrixA, const SquareMatrix<T>& matrixB) {
    size_t n = matrixA.size();

    if(n == 1) {
        return {matrixA.at(0, 0) * matrixB.at(0, 0)};
    }
    SquareMatrix<T> matrixC(n);
    size_t m = n / 2;

    SquareMatrix<T> A11(m), A12(m), A21(m), A22(m);
    SquareMatrix<T> B11(m), B12(m), B21(m), B22(m);

     for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < m; j++) {
            A11.at(i, j) = matrixA.at(i, j);
            A12.at(i, j) = matrixA.at(i, j + m);
            A21.at(i, j) = matrixA.at(i + m, j);
            A22.at(i, j) = matrixA.at(i + m, j + m);

            B11.at(i, j) = matrixB.at(i, j);
            B12.at(i, j) = matrixB.at(i, j + m);
            B21.at(i, j) = matrixB.at(i + m, j);
            B22.at(i, j) = matrixB.at(i + m, j + m);
        }
    }

    SquareMatrix<T> C11(m), C12(m), C21(m), C22(m);

    if (n <= 256) {
        auto P1 = strassen(A11, B12 - B22);
        auto P2 = strassen(A11 + A12, B22);
        auto P3 = strassen(A21 + A22, B11);
        auto P4 = strassen(A22, B21 - B11);
        auto P5 = strassen(A11 + A22, B11 + B22);
        auto P6 = strassen(A12 - A22, B21 + B22);
        auto P7 = strassen(A11 - A21, B11 + B12);

        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < m; j++) {
                C11.at(i, j) = P5.at(i, j) + P4.at(i, j) - P2.at(i, j) + P6.at(i, j);
                C12.at(i, j) = P1.at(i, j) + P2.at(i, j);
                C21.at(i, j) = P3.at(i, j) + P4.at(i, j);
                C22.at(i, j) = P5.at(i, j) + P1.at(i, j) - P3.at(i, j) - P7.at(i, j);
            }
        }
    } else {
        auto P1 = std::async([&](){ return strassen(A11, B12 - B22);});
        auto P2 = std::async([&](){ return strassen(A11 + A12, B22);});
        auto P3 = std::async([&](){ return strassen(A21 + A22, B11);});
        auto P4 = std::async([&](){ return strassen(A22, B21 - B11);});
        auto P5 = std::async([&](){ return strassen(A11 + A22, B11 + B22);});
        auto P6 = std::async([&](){ return strassen(A12 - A22, B21 + B22);});
        auto P7 = std::async([&](){ return strassen(A11 - A21, B11 + B12);});

        P1.wait(); P2.wait(); P3.wait(); P4.wait(); P5.wait(); P6.wait(); P7.wait();

        auto P1_result = P1.get();
        auto P2_result = P2.get();
        auto P3_result = P3.get();
        auto P4_result = P4.get();
        auto P5_result = P5.get();
        auto P6_result = P6.get();
        auto P7_result = P7.get();

        for (size_t i = 0; i < m; i++) {
            for (size_t j = 0; j < m; j++) {
                C11.at(i, j) = P5_result.at(i, j) + P4_result.at(i, j) - P2_result.at(i, j) + P6_result.at(i, j);
                C12.at(i, j) = P1_result.at(i, j) + P2_result.at(i, j);
                C21.at(i, j) = P3_result.at(i, j) + P4_result.at(i, j);
                C22.at(i, j) = P5_result.at(i, j) + P1_result.at(i, j) - P3_result.at(i, j) - P7_result.at(i, j);
            }
        }
    }

    for(int i=0; i<m; i++) {
        for(int j=0; j<m; j++) {
            matrixC.at(i, j) = C11.at(i, j);
            matrixC.at(i, j + m) = C12.at(i, j);
            matrixC.at(i + m, j) = C21.at(i, j);
            matrixC.at(i + m, j + m) = C22.at(i, j);
        }
    }

    return matrixC;
}

template <class T>
SquareMatrix<T> gauss(const SquareMatrix<T>& a, const SquareMatrix<T>& b) {
    auto n = a.size();

    SquareMatrix<T> c(a);
    for(size_t i=0; i<n; i++) {
        for(size_t j=0; j<n; j++) {
           c.at(i, j) = 0;
           for(size_t k = 0; k < n; k++)
                c.at(i, j) += a.at(i, k) * b.at(k, j);
        }
    }
    return c;
}

template <class T>
SquareMatrix<T> generateSquareMatrix(size_t size) {
    srand(time(NULL));
    SquareMatrix<T> m(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            m.at(i, j) = static_cast<T>((rand() % 20) - 10);
        }
    }
    return m;
}

int main()
{
    // SquareMatrix<int> matrixA{2,4,5,6,
    //                           1,2,3,4,
    //                           5,7,8,9,
    //                           8,6,5,4};


    // SquareMatrix<int> matrixB{2,4,5,6,
    //                           1,2,3,4,
    //                           5,7,8,9,
    //                           8,6,5,4};
    auto matrixA = generateSquareMatrix<int>(512);
    auto matrixB = generateSquareMatrix<int>(512);

    using namespace std::chrono;

    std::cout << "Strassen" << std::endl;
    auto begin = steady_clock::now();
    auto matrixC = strassen(matrixA , matrixB);
    auto end = steady_clock::now();

    // auto n = matrixC.size();
    // for(auto i=0; i < n; i++) {
    //     for(auto j=0; j < n; j++) {
    //         std::cout << std::setw(3) << matrixC.at(i, j) << " ";
    //     }
    //     std::cout<<std::endl;
    // }
    std::cout << "Time: "
              << duration_cast<microseconds>(end - begin).count() << "µs" << std::endl;

//---------------------------
    std::cout<<"\nGauss"<<std::endl;
    begin = steady_clock::now();
    auto matrixD = gauss(matrixA , matrixB);
    end = steady_clock::now();

    // for(int i = 0; i < n; i++) {
    //     for(int j = 0; j < n; j++) {
    //         std::cout << std::setw(3) << matrixD.at(i, j) << " ";
    //     }
    //     std::cout<<std::endl;
    // }
    std::cout << "Time: "
          << duration_cast<microseconds>(end - begin).count() << "µs" << std::endl;

    return 0;
}

// SUMMARY: The more processors are used to calculate Strassen algorithm the faster it can be
//          MacBook Pro 14 (core_i5 2physical:4): single thread - ratio is 33:1, multithread - 16:1
//              slightly more than 2 times faster with multithreading approach
//          MacBook Air (M1 4fast:4slow): single thread - ratio is 41:1, multithread - 9:1
//              about 5 times faster with multithreading approach

/*  WITHOUT O       WITH O
size 4
89:1                39:1
Strassen
 81  87  92  97
 51  53  55  57
129 144 155 166
 79 103 118 133
Time: 178µs         78µs

Gauss
 81  87  92  97
 51  53  55  57
129 144 155 166
 79 103 118 133
Time: 2µs           2µs
*/
/*
size 16
99:1                44:1
Strassen
Time: 10135µs       3941µs

Gauss
Time: 102µs         89µs
*/
/*
size 128
90:1                29:1
Strassen
Time: 2230421µs     858952µs

Gauss
Time: 24721µs       29456µs
*/
/*
size 256
n/a                 25:1
Strassen
Time: n/a           6728213µs

Gauss
Time: n/a           267672µs
*/
/*
size 512
n/a                 5:1
Strassen
Time: n/a           59899215µs          55729963µs

Gauss
Time: n/a           12689412µs          1912256µs
*/

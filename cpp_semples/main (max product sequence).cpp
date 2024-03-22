/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <cassert>
#include <functional>
#include <cstdlib>
#include <chrono>

#define assertm(exp, msg) assert(((void)msg, exp))

#include <vector>
#include <limits>
#include <algorithm>

//-------------SOLUTION 1------------------

// This algorithm is based on the naive enumeration method via recursion. 
// Briefly speacking it enumarates all the combinations for multLength.
template <typename T>
std::vector<T> maxProductSequence_v1(const std::vector<T>& in, size_t multLength) {
    
    if (multLength > in.size())
        return {};
    
    T max = std::numeric_limits<T>::min();
    std::vector<T> result;
    result.shrink_to_fit();

    auto func = [&max, &result, &in, multLength] (auto& itself, auto idx,
                                                  auto depth, auto product,
                                                  auto idxs) -> void {
        if (!depth) {
            if (product > max) {
                max = product;
                result.swap(idxs);
            }
            return;
        }

        for (auto i = idx; i <= in.size() - depth; ++i) {
            auto copyProduct = product * in[i];
            auto copyIdxs(idxs);
            copyIdxs.push_back(in[i]);

            itself(itself, i + 1, depth - 1, copyProduct, std::move(copyIdxs));
        }
        
        return;
    };
    
    std::vector<T> tempIdxBuff;
    tempIdxBuff.reserve(multLength);
    func(func, 0, multLength, 1, std::move(tempIdxBuff));
    
    return result;
}

//-------------SOLUTION 2------------------

// Ususaly faster on small to big amount of data
// so it's a choice by default
template <class T, class Comp>
struct SortedVStrategy {
    template <class U>
    void push(std::vector<T>& vec, U&& val);
    void pop(std::vector<T>& vec);
    
    typename std::vector<T>::const_reference
    top(const std::vector<T>& vec) const;
    
    void sort(std::vector<T>& vec);
};

template <class T, class Comp> template <class U>
void SortedVStrategy<T, Comp>::push(std::vector<T>& vec, U&& val) {
    vec.insert(
        std::upper_bound(vec.begin(), vec.end(), val, Comp{}),
        val
    );
}

template <class T, class Comp>
void SortedVStrategy<T, Comp>::pop(std::vector<T>& vec) {
    vec.pop_back();
}

template <class T, class Comp>
typename std::vector<T>::const_reference
SortedVStrategy<T, Comp>::top(const std::vector<T>& vec) const {
    return vec.front();
}

template <class T, class Comp>
void SortedVStrategy<T, Comp>::sort(std::vector<T>& vec) {
    return;
}

// May be faster on very large amount of data
// due to less increising complexity than in case 
// a sorted SortedVStrategy
template <class T, class Comp>
struct HeapVStrategy {
    template <class U>
    void push(std::vector<T>& vec, U&& val);
    void pop(std::vector<T>& vec);
    
    typename std::vector<T>::const_reference
    top(const std::vector<T>& vec) const;
    
    void sort(std::vector<T>& vec);
};

template <class T, class Comp> template <class U>
void HeapVStrategy<T, Comp>::push(std::vector<T>& vec, U&& val) {
    vec.push_back(std::forward<U>(val));
    std::push_heap(vec.begin(), vec.end(), Comp{});
}

template <class T, class Comp>
void HeapVStrategy<T, Comp>::pop(std::vector<T>& vec) {
    std::pop_heap(vec.begin(), vec.end(), Comp{});
    vec.pop_back();
}

template <class T, class Comp>
typename std::vector<T>::const_reference
HeapVStrategy<T, Comp>::top(const std::vector<T>& vec) const {
    return vec.front();
}

template <class T, class Comp>
void HeapVStrategy<T, Comp>::sort(std::vector<T>& vec) {
    std::sort_heap(vec.begin(), vec.end(), Comp{});
    // std::sort(vec.begin(), vec.end(), Comp{}); // INFO: may be a bit better...
}

template <
            class T,
            class Strtgy = HeapVStrategy<
                T,
                std::less<typename std::vector<T>::value_type>
            >
         >
class CustomQueue : private std::vector<T> { // TODO: Generalize by Container
    Strtgy m_strategy;
    
public:
    using const_reference = typename std::vector<T>::const_reference;
    const_reference operator[](size_t pos) const { return std::vector<T>::operator[](pos); }

    using std::vector<T>::size;
    using std::vector<T>::empty;
    using std::vector<T>::reserve;
    using std::vector<T>::shrink_to_fit;

    template <class U>
    void push(U&& val);
    
    void pop();
    const_reference top() const;
    void sort();
};

template <class T, class Strtgy> template <class U>
void CustomQueue<T, Strtgy>::push(U&& val) {
    m_strategy.push(*this, std::forward<U>(val));
}

template <class T, class Strtgy>
void CustomQueue<T, Strtgy>::pop() {
    m_strategy.pop(*this);
}

template <class T, class Strtgy>
typename CustomQueue<T, Strtgy>::const_reference
CustomQueue<T, Strtgy>::top() const {
    return m_strategy.top(*this);
}

template <class T, class Strtgy>
void CustomQueue<T, Strtgy>::sort() {
    m_strategy.sort(*this);
}

template <typename T>
std::vector<T> maxProductSequence_v2(const std::vector<T>& in, size_t multLength) {
    if (multLength > in.size())
        return {};
        
    // Step1: find "multLength" maximum elements and (multLength - (multLength % 2)) 
    //        minimum elements from the "in" vector
    const size_t minCount = multLength - multLength % 2;
    const size_t maxCount = multLength;

    // using MinQueue = CustomQueue<T>;
    // using MaxQueue = CustomQueue<T, HeapVStrategy<T, std::greater<int>>>;
    using MinQueue = CustomQueue<T, SortedVStrategy<T, std::less<int>>>;
    using MaxQueue = CustomQueue<T, SortedVStrategy<T, std::greater<int>>>;
    
    MinQueue minQueue;
    MaxQueue maxQueue;
    minQueue.reserve(minCount);
    maxQueue.reserve(maxCount);
    
    for (auto i = 0; i < in.size(); ++i) {
        minQueue.push(in[i]);
        maxQueue.push(in[i]);
        
        if (minQueue.size() > minCount)
            minQueue.pop();
            
        if (maxQueue.size() > maxCount)
            maxQueue.pop();
    }
    
    // Step2: generate all possible combinations of a product of minimum and 
    //        maximum numbers found in the previous stage
    minQueue.sort();
    maxQueue.sort();
    
    // adding chache reduces an additional complexity of 2*O(n*log(n))
    auto maxCache = 1;
    auto maxCacheIdx = 0;
    
    // TODO: try to improve it the section below by using division 
    //       but be carefull with the 0 case.
    std::vector<T> result;
    auto maxProd = std::numeric_limits<T>::min();
    for (int i = minQueue.size(); i >= 0; i-=2) {
        std::vector<T> curr;
        curr.reserve(multLength);
        auto prod = 1;
        for (auto j = 0; j < i; ++j) {
            curr.push_back(minQueue[j]);
            prod *= minQueue[j];
        }        
        prod *= maxCache;
        for (auto j = maxCacheIdx; j < (multLength - i); ++j) {
            curr.push_back(maxQueue[j]);
            prod *= maxQueue[j];
            maxCacheIdx = j;
        }

        if (prod > maxProd) {
            result.swap(curr);
            maxProd = prod;
        }
    }
    
    return result;
}

//-------------TESTING CODE-----------------------------------------

template <class T>
bool checkVecRes(const std::vector<T>& l, const std::vector<T>& r) {
    bool found {false};
    for (auto itL = l.begin(); itL != l.end(); ++itL) {
        for (auto itR = r.begin(); itR != r.end(); ++itR)
            if (*itR == *itL)
               found = true;
            
        if (!found)
            return false;
        else
            found = false;
    }
    return true;
}

template <class T>
void testFunc(std::function<std::vector<T>(const std::vector<T>&, size_t)> func) {
    using namespace std; 
    vector<pair<size_t, vector<int>>> inValues = {
                                        {2, {2, 3, -2, 4}}, // #1
                                        {2, {-1, -2, -3, -4}}, // #2
                                        {2, {-1, -200, 357, -4}}, // #3
                                        {3, {-1, -200, 357, -4}}, // #4
                                        {1, {0, 1, 2, 3, 4}}, // #5
                                        {5, {0, 1, 2, 3, 4}}, // #6
                                        {0, {0, 1, 2, 3, 4}}, // #7
                                        // {-1, {0, 1, 2, 3, 4}}, // #8
                                        {3, {1, -2, -3, 4, 5}}, // #9
                                        {3, {1, -200, 357, 4}}, // #10
                                        {5, {1, -200, 357, 4}}, // #11
                                        {3, {2, 8, 3, -3, -3, -9, -9, -9, -8, -5}} // #12
                                    };
                                    
    vector<vector<int>> outValues = {
                                        {3, 4}, // #1
                                        {-3, -4}, // #2
                                        {-200, -4}, // #3
                                        {-200, 357, -4}, // #4
                                        {4}, // #5
                                        {0, 1, 2, 3, 4},  // #6
                                        {}, // #7
                                        // {}, // #8
                                        {-2, -3, 5}, // #9
                                        {1, 357, 4}, // #10
                                        {}, // #11
                                        {8, -9, -9} // #12
                                    };
    {
        auto inIt = inValues.begin();
        auto outIt = outValues.begin();
        for (; inIt != inValues.end() && outIt != outValues.end();
             ++inIt, ++outIt) {
                 auto retVal = func(inIt->second, inIt->first);
                 
                 cout << "INPUT VALUE: \"len: " << inIt->first << ", {";
                 
                 for (const auto& val : inIt->second)
                     cout << val << " ";
                    
                 cout << "}\"; EXPECTED VALUE: \"{";
                 
                 for (const auto& val : *outIt)
                    cout << val << " ";
                    
                 cout << "}\"; ACTUAL VALUE: \"{";
                 
                 for (const auto& val : retVal)
                    cout << val << " ";
                    
                 cout << "}\"" << endl;
                      
                //  assertm(retVal == *outIt, "----Test-case Failed----");
                 assertm(checkVecRes(*outIt, retVal), "----Test-case Failed----");
                 cout << "TEST-CASE PASSED!" << endl;
                 cout << endl;
        }
    }
    
    return;
}

template <class T>
void checkPerf(std::function<std::vector<T>(const std::vector<T>&, size_t)> func) {
    srand(static_cast<unsigned>(time(nullptr)));

    constexpr T range {20}, offset {range / 2};

    // std::array<int, 4> counts = {10, 100, 1000, 10000};
    std::array<int, 1> counts = {50000};
    std::vector<std::vector<int>> results;
    results.reserve(counts.size());

    for (const auto& count : counts) {
        std::vector<T> vec;
        vec.reserve(count);
        for (auto i = 0; i < count; ++i)
            vec.push_back((rand() % range) - offset);
            
        results.push_back(std::move(vec));
    }

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    using std::chrono::microseconds;
    
    std::cout << "---PERFORMANCE TEST---" << std::endl;

    for (const auto& vec : results) {
        std::cout << "size: " << vec.size() << std::endl;

        const std::array<size_t, 3> ks = {3, vec.size() / 2, vec.size() - 1};
        // const std::array<size_t, 1> ks = {2};
        for (const auto& k : ks) {
            std::cout << "  k: " << k << std::endl;
            
            auto t1 = high_resolution_clock::now();
            
            func(vec, k);

            auto t2 = high_resolution_clock::now();
            auto ms_int = duration_cast<milliseconds>(t2 - t1);
            auto us_int = duration_cast<microseconds>(t2 - t1);
    
            if (ms_int.count() != 0)
                std::cout << "  total time: " << ms_int.count() << "ms" << std::endl;
            else
                std::cout << "  total time: " << us_int.count() << "us" << std::endl;
    
        }
    }
}

int main()
{
    // {
    //     std::vector<int> vec { -1, -200, 357, -4};
    //     maxProductSequence_v2<int>(vec, 3);
    // }
    // {
    //     std::vector<int> vec { -1, -2, -3, 4, 5, 9, -7};
    //     maxProductSequence_v2<int>(vec, 3);
    // }
    // {
    //     std::vector<int> vec { -1, -2, -3, 4, 5, 9, -7, -10, -20, -3, -4, 5, 90, -7};
    //     maxProductSequence_v2<int>(vec, 6);
    // }
    
    // testFunc<int>(maxProductSequence_v1<int>);
    testFunc<int>(maxProductSequence_v2<int>);
    
    // INFO: Disabled due to the O(n!) complexity
    // checkPerf<int>(maxProductSequence_v1<int>);
    checkPerf<int>(maxProductSequence_v2<int>);
    
    return 0;
}


/* TIMING RESULTS:

HeapV + std::sort:
size: 50000
  k: 3
  total time: 22ms	    33ms
  k: 25000
  total time: 6442ms	9468ms
  k: 49999
  total time: 25528ms	37634ms

HeapV:
size: 50000
  k: 3
  total time: 26ms	    33ms	25ms
  k: 25000
  total time: 7713ms	9537ms	6449ms
  k: 49999
  total time: 30005ms	37716ms	25051ms

SortedV:
size: 50000
  k: 3
  total time: 15ms	    12ms	15ms    12ms
  k: 25000
  total time: 7621ms	6483ms	7545ms  6388ms
  k: 49999
  total time: 29950ms	26093ms	30069ms 25527ms
  
*/
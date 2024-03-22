#include <limits>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <vector>
#include <list>

#include <iostream>
#include <deque>
#include <random>
#include <chrono>
#include <cmath>


// TODO: implement the sorted vector structure with different sorting engines

namespace prvt {
template <typename T>
struct EmptyExtractor {
    inline T operator()(T t) {
        return t;
    }
};

template <typename T>
T getDigit(T t, uint8_t d) {
    constexpr uint8_t base = 10;
    return static_cast<T>(t / pow(base, d)) % base;
}

template<typename, typename = void>
struct has_reserve : std::false_type { };

template<typename T>
struct has_reserve<T,
           std::void_t<decltype(T::reserve)>
       > : std::true_type { };

template <typename Container,
           typename std::enable_if_t<
               has_reserve<Container>::value, bool
           > = true>
void reserve(Container& c, size_t n) {
    c.reserve(n);
}

template <typename Container,
            typename std::enable_if_t<
                !has_reserve<Container>::value, bool
            > = true>
void reserve(Container&, size_t) { }
}

template <
           typename Container,
           typename It = typename Container::iterator,
           typename T = typename std::iterator_traits<It>::value_type,
           typename ExtractFunc = prvt::EmptyExtractor<T>,
           typename KeyT = decltype(ExtractFunc()(T()))
         >
         //TODO: Add min and max value in order to shrinken the range for memory economy
Container countingSort(const Container& c, KeyT k, ExtractFunc extract = ExtractFunc()) {
    // Compile-time requirements:
    static_assert(std::is_same_v<
               typename std::iterator_traits<It>::iterator_category,
               std::random_access_iterator_tag
          >, "Only containers with random access iterators are accepted");
    static_assert(std::is_integral_v<KeyT>, "Type KeyT must be integral");
    // signed type cannot be used for indexing
    static_assert(!std::numeric_limits<KeyT>::is_signed, "Signed types cannot be accepted");

    // temporary container for counting
    std::vector<typename Container::size_type> tmp (k + 1, 0);
    // output container for sorted values
    Container out (c.size(), 0);

    for (const auto& v : c)
        ++tmp[extract(v)];

    for (T i = 0; i < k; ++i)
        tmp[i + 1] = tmp[i] + tmp[i + 1];

    for (const auto& v : c | std::views::reverse) {
        out[tmp[extract(v)] - 1] = v;
        --tmp[extract(v)];
    }

    return out;
}

// TODO: check this algorithm
void radixSort_2(int* a, int N){
    using namespace std;

    const int INT_BIT_SIZE = sizeof(int)<<3, RADIX = 0x100, MASK = RADIX-1, MASK_BIT_LENGTH = 8;
    int *result = new int[N](), *buckets = new int[RADIX](), *startIndex = new int[RADIX](), *temp = nullptr;
    int flag = 0, key = 0;
    bool hasNeg = false;
    while (flag < INT_BIT_SIZE){
        for (int i = 0; i < N; ++i) {
            key = (a[i] & (MASK << flag)) >> flag;
            if(key < 0){
                key += MASK;
                hasNeg = true;
            }
            ++buckets[key];
        }
        startIndex[0] = 0;
        for (int j = 1; j < RADIX; ++j) startIndex[j] = startIndex[j - 1] + buckets[j - 1];
        for (int i = N-1; i >= 0; --i){
            key = (a[i] & (MASK << flag)) >> flag;
            if(key < 0) key += MASK;
            result[startIndex[key] + --buckets[key]] = a[i];
        }
        memcpy(a,result,N*sizeof(int));
        flag += MASK_BIT_LENGTH;
    }
    if(hasNeg){
        int indexOfNeg = 0;
        for (int i = 0; i < N; i++) {
            if(a[i] < 0) {
                indexOfNeg = i;
                break;
            }
        }
        memcpy(a,result+indexOfNeg,(N-indexOfNeg)*sizeof(int));
        memcpy(a+(N-indexOfNeg),result,indexOfNeg*sizeof(int));
    }
    delete[] result;
    delete[] buckets;
    delete[] startIndex;
}

template <
            typename Container,
            typename It = typename Container::iterator,
            typename T = typename std::iterator_traits<It>::value_type,
            typename ExtractFunc = prvt::EmptyExtractor<T>,
            typename KeyT = decltype(ExtractFunc()(T()))
         >
void radixSort(Container& c, uint8_t d, ExtractFunc extract = ExtractFunc()) {
    // Compile-time requirements:
    static_assert(std::is_same_v<
               typename std::iterator_traits<It>::iterator_category,
               std::random_access_iterator_tag
          >, "Only containers with random access iterators are accepted");
    static_assert(std::is_integral_v<KeyT>, "Type KeyT must be integral");
    // signed type cannot be used for indexing
    static_assert(!std::numeric_limits<KeyT>::is_signed, "Signed types cannot be accepted");

    constexpr uint8_t k = 9;

    std::array<typename Container::size_type, k + 1> tmp;
    for (uint8_t i = 0; i < d; ++i) {
        std::fill(tmp.begin(), tmp.end(), 0);
        Container out(c.size(), 0);

        for (const auto& v : c)
            ++tmp[prvt::getDigit(v, i)];

        for (T i = 0; i < k; ++i)
            tmp[i + 1] = tmp[i] + tmp[i + 1];

        for (const auto& v : c | std::views::reverse) {
            out[tmp[prvt::getDigit(extract(v), i)] - 1] = v;
            --tmp[prvt::getDigit(extract(v), i)];
        }

        c.swap(out);
    }
}

// This is O(n^2) but used here for the auxiliary reason
template<typename It>
void insertion_sort(It begin, It end) {
    if (begin == end) {
        return;
    }

    for (auto j = std::next(begin); j != end; ++j)  {
        auto key = *j;
        auto i = std::prev(j);

        while (*i > key) {
            *std::next(i) = *i;
            i = std::prev(i);
        }
        *std::next(i) = key;
    }
}

// List version of the bucket sort
template<typename T>
std::list<T> bucketSort_List(std::vector<T> & array, T max, size_t k) {
   std::vector<std::list<T>> temp(k, std::list<T>()); // FIXME: check if k or k+1 is correct
   std::list<T> result;
   for(const auto& it: array) {
      size_t ind = (k - 1) * it / max;
      temp[ind].push_back(it);
   }

   for(int i = 0; i < k; ++i) {
      temp[i].sort();
      result.merge(temp[i]);
   }

   return result;
}

// Vector version of the bucket sort
template <
            typename Container,
            typename It = typename Container::iterator,
            typename T = typename std::iterator_traits<It>::value_type,
            typename ExtractFunc = prvt::EmptyExtractor<T>,
            typename KeyT = decltype(ExtractFunc()(T()))
         >
Container bucketSort(const Container& c, KeyT max, size_t k, ExtractFunc extract = ExtractFunc()) {
    // Compile-time requirements:
    static_assert(std::is_same_v<
               typename std::iterator_traits<It>::iterator_category,
               std::random_access_iterator_tag
          >, "Only containers with random access iterators are accepted");

    std::vector<std::vector<T>> temp(k);
    Container result;
    prvt::reserve(result, c.size());

    for(const auto& it: c) {
      size_t ind = (k - 1) * extract(it) / max;
      temp[ind].push_back(it);
    }

    for(size_t i = 0; i < k; ++i) {
      std::sort(temp[i].begin(), temp[i].end(),
            [](auto a, auto b) {
                 ExtractFunc extract;
                 return extract(a) < extract(b);
            });
      result.insert(
          result.end(),
          std::make_move_iterator(temp[i].begin()),
          std::make_move_iterator(temp[i].end())
        );
    }

    return result;
}

template <
            typename Container,
            typename It = typename Container::iterator,
            typename T = typename std::iterator_traits<It>::value_type,
            typename ExtractFunc = prvt::EmptyExtractor<T>,
            typename KeyT = decltype(ExtractFunc()(T()))
         >
Container pigeonholeSort(const Container& c, KeyT min, KeyT max, ExtractFunc extract = ExtractFunc()) {
    // Compile-time requirements:
    static_assert(std::is_same_v<
               typename std::iterator_traits<It>::iterator_category,
               std::random_access_iterator_tag
          >, "Only containers with random access iterators are accepted");
    static_assert(std::is_integral_v<KeyT>, "Type KeyT must be integral");
    // signed type cannot be used for indexing
    static_assert(!std::numeric_limits<KeyT>::is_signed, "Signed types cannot be accepted");

    std::vector<std::vector<T>> temp(max - min + 1);
    Container result;
    prvt::reserve(result, c.size());

    for(const auto& it: c) {
      temp[extract(it) - min].push_back(it);
    }

    for(size_t i = 0; i <= max - min; ++i) {
      result.insert(
          result.end(),
          std::make_move_iterator(temp[i].begin()),
          std::make_move_iterator(temp[i].end())
        );
    }

    return result;
}

template <
            typename Container,
            typename It = typename Container::iterator,
            typename T = typename std::iterator_traits<It>::value_type,
            typename ExtractFunc = prvt::EmptyExtractor<T>,
            typename KeyT = decltype(ExtractFunc()(T()))
         >
Container pigeonholeSortForUniqueKeys(const Container& c, KeyT min, KeyT max, ExtractFunc extract = ExtractFunc()) {
    // Compile-time requirements:
    static_assert(std::is_same_v<
               typename std::iterator_traits<It>::iterator_category,
               std::random_access_iterator_tag
          >, "Only containers with random access iterators are accepted");
    static_assert(std::is_integral_v<KeyT>, "Type KeyT must be integral");
    // signed type cannot be used for indexing
    static_assert(!std::numeric_limits<KeyT>::is_signed, "Signed types cannot be accepted");

    std::vector<T> result(max - min + 1, std::numeric_limits<KeyT>::max());
    for(const auto& it: c) {
        result[extract(it) - min] = it;
    }

    result.erase(std::remove_if(result.begin(), result.end(),
        [] (int e) {
            return e == std::numeric_limits<T>::max();
        }
    ), result.end());

    return result;
}

//------------------------------TEST--------------------------------

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

template <typename TimeCountType>
void testFunc(long count, const std::string& timeCount) {
    // generate a random array
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<uint8_t> data (count, 0);
    std::generate(data.begin(), data.end(), g);

    auto maxVal = std::max_element(std::begin(data), std::end(data));
    auto t1 = high_resolution_clock::now();
    auto sortedData = countingSort(data, *maxVal);
    auto t2 = high_resolution_clock::now();

    auto dataCopy = data;
    auto t3 = high_resolution_clock::now();
    std::sort(dataCopy.begin(), dataCopy.end());
    auto t4 = high_resolution_clock::now();

    auto dataCopy2 = data;
    // counting a number of digits in the max value of the type
    // however it may be faster to calculate it for a maximum value of an input array
    const uint8_t d = floor(log10(std::numeric_limits<uint8_t>::max()) + 1);
    auto t5 = high_resolution_clock::now();
    radixSort(dataCopy2, d);
    auto t6 = high_resolution_clock::now();

    // Imperically it turned out to be the best number of buckets
    auto buckets = sqrt(data.size());
    // It turned out that list version is too slow on the big size of data
    if (count <= 1'000'000)
        auto result1 = bucketSort_List(data, *maxVal, buckets);
    auto t7 = high_resolution_clock::now();

    auto result2 = bucketSort(data, *maxVal, buckets);
    auto t8 = high_resolution_clock::now();

    auto minVal = std::min_element(std::begin(data), std::end(data));
    auto t9 = high_resolution_clock::now();
    auto result3 = pigeonholeSort(data, *minVal, *maxVal);
    auto t10 = high_resolution_clock::now();

    auto result4 = pigeonholeSortForUniqueKeys(data, *minVal, *maxVal);
    auto t11 = high_resolution_clock::now();

    auto countingSortT = duration_cast<TimeCountType>(t2 - t1);
    auto introSortT = duration_cast<TimeCountType>(t4 - t3);
    auto radixSortT = duration_cast<TimeCountType>(t6 - t5);
    TimeCountType bucketSort_v1;
    if (count <= 1'000'000)
        bucketSort_v1 = duration_cast<TimeCountType>(t7 - t6);
    auto bucketSort_v2 = duration_cast<TimeCountType>(t8 - t7);
    auto pigeonholeSortT = duration_cast<TimeCountType>(t10 - t9);
    auto pigeonholeSortUniquT = duration_cast<TimeCountType>(t11 - t10);

    std::cout << "Counting sort: " << countingSortT.count() << timeCount << std::endl;
    std::cout << "STD intro-sort: " << introSortT.count() << timeCount << std::endl;
    std::cout << "Radix sort: " << radixSortT.count() << timeCount << std::endl;
    if (count <= 1'000'000)
        std::cout << "Bucket sort list version: " << bucketSort_v1.count() << timeCount << std::endl;
    std::cout << "Bucket sort vector version: " << bucketSort_v2.count() << timeCount << std::endl;
    std::cout << "Pigeonhole sort: " << pigeonholeSortT.count() << timeCount << std::endl;
    std::cout << "Pigeonhole sort for unique keys: " << pigeonholeSortUniquT.count() << timeCount << std::endl;
}

int main() {
    {
        std::cout << "\n---------------------100'000'000------------------: " << std::endl;
        testFunc<milliseconds>(100'000'000, "ms");

        std::cout << "\n---------------------1'000'000--------------------: " << std::endl;
        testFunc<milliseconds>(1'000'000, "ms");

        std::cout << "\n---------------------1'000------------------------: " << std::endl;
        testFunc<microseconds>(1'000, "us");

        std::cout << "\n---------------------10---------------------------: " << std::endl;
        testFunc<nanoseconds>(10, "ns");
    }

    // {
    //     // std::vector<double> testData({0.74,0.45,0.84,0.63,0.48,0.24,0.75,0.12});
    //     // std::vector<uint8_t> testData {2, 48, 0, 4, 0, 5, 125, 1};
    //     std::vector<uint8_t> testData {2, 4, 6, 7, 0, 15, 12, 114};
    //
    //     auto maxVal = std::max_element(std::begin(testData), std::end(testData));
    //     auto minVal = std::min_element(std::begin(testData), std::end(testData));
    //     // auto result = pigeonholeSort(testData, *minVal, *maxVal);
    //     auto result = pigeonholeSortForUniqueKeys(testData, *minVal, *maxVal);
    //     // auto result = countingSort(testData, *maxVal);
    //     // radixSort(testData, floor(log10(std::numeric_limits<uint8_t>::max()) + 1));
    //     // auto result = bucketSort(testData, *maxVal, sqrt(testData.size()));
    //
    //     for(const auto &it: result) {
    //         std::cout << static_cast<int>(it) << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

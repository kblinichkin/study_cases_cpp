weqwd
//----------------------------------------TEST--------------------

template <typename It, typename SizeType = std::iterator_traits<It>::difference_type>
void testFunc(It begin, It end, SizeType k) {
    auto pos = randomSelect(begin, end, k);

    std::sort(begin, end);
    std::for_each(begin, end, [](auto v){
        std::cout << static_cast<int>(v) << " ";
    });
    std::cout << std::endl;

    std::cout << "k-th smallest element: " << k << std::endl;
    std::cout << "value: " << static_cast<int>(*pos) << std::endl;
}

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

template <typename TimeCountType>
void testFunc(long count, const std::string& timeCount) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<uint8_t> data (count, 0);
    std::generate(data.begin(), data.end(), g);
    constexpr std::vector<uint8_t>::size_type k = 5;

    std::vector<uint8_t> data1(data);

    auto t1 = high_resolution_clock::now();
    auto res1 = randomSelect(data.begin(), data.end(), k);
    auto t2 = high_resolution_clock::now();

    std::sort(data1.begin(), data1.end());
    auto res2 = data1[k + 1];
    auto t3 = high_resolution_clock::now();

    auto findMinMaxT = duration_cast<TimeCountType>(t2 - t1);
    auto stdMinMaxT = duration_cast<TimeCountType>(t3 - t2);

    std::cout << "randomSelect ---- " << std::endl;
    std::cout << "ids: " << k << std::endl;
    std::cout << "value: " << static_cast<int>(*res1) << std::endl;
    std::cout << "time: " << findMinMaxT.count() << timeCount << std::endl;
    std::cout << "select with std::sort ---- " << std::endl;
    std::cout << "ids: " << k << std::endl;
    std::cout << "value: " << static_cast<int>(res2) << std::endl;
    std::cout << "time: " << stdMinMaxT.count() << timeCount << std::endl;
    std::cout << "-------------------------" << std::endl;
}

template <typename TimeCountType>
void testFunc1(long count, const std::string& timeCount) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<uint8_t> data (count, 0);
    std::generate(data.begin(), data.end(), g);

    auto t1 = high_resolution_clock::now();
    auto res = findMinMax(data.begin(), data.end());
    auto t2 = high_resolution_clock::now();

    auto res1 = std::max_element(data.begin(), data.end());
    auto res2 = std::min_element(data.begin(), data.end());
    auto t3 = high_resolution_clock::now();

    auto findMinMaxT = duration_cast<TimeCountType>(t2 - t1);
    auto stdMinMaxT = duration_cast<TimeCountType>(t3 - t2);

    std::cout << "findMinMax ---- " << std::endl;
    std::cout << "min: " << static_cast<int>(*res.first) << std::endl;
    std::cout << "max: " << static_cast<int>(*res.second) << std::endl;
    std::cout << "time: " << findMinMaxT.count() << timeCount << std::endl;
    std::cout << "std min/max ---- " << std::endl;
    std::cout << "min: " << static_cast<int>(*res2) << std::endl;
    std::cout << "max: " << static_cast<int>(*res1) << std::endl;
    std::cout << "time: " << stdMinMaxT.count() << timeCount << std::endl;
    std::cout << "-------------------------" << std::endl;
}

int main() {
    // {
    //     std::vector<uint8_t> testData {2, 48, 0, 4, 0, 5, 125, 1};
    //     constexpr std::vector<uint8_t>::size_type k = 5;
    //
    //     testFunc(testData.begin(), testData.end(), k);
    // }
    {
        std::cout << "\n---------------------10'000'000------------------: " << std::endl;
        testFunc<milliseconds>(10'000'000, "ms");

        std::cout << "\n---------------------1'000'000--------------------: " << std::endl;
        testFunc<milliseconds>(1'000'000, "ms");

        std::cout << "\n---------------------1'000------------------------: " << std::endl;
        testFunc<microseconds>(1'000, "us");

        std::cout << "\n---------------------10---------------------------: " << std::endl;
        testFunc<nanoseconds>(10, "ns");
    }
    {
        std::cout << "\n---------------------100'000'000------------------: " << std::endl;
        testFunc1<milliseconds>(100'000'000, "ms");

        std::cout << "\n---------------------1'000'000--------------------: " << std::endl;
        testFunc1<microseconds>(1'000'000, "us");

        std::cout << "\n---------------------1'000------------------------: " << std::endl;
        testFunc1<nanoseconds>(1'000, "ns");

        std::cout << "\n---------------------10---------------------------: " << std::endl;
        testFunc1<nanoseconds>(10, "ns");
    }
}

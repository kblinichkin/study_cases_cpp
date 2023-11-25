#include <map>
#include <iostream>
#include <tuple>
#include <cassert>


template<typename K, typename V>
class interval_map {
    friend void IntervalMapTest();
public:
    V m_valBegin;
    std::map<K,V> m_map;
public:
    // constructor associates whole range of K with val
    interval_map(V const& val)
        : m_valBegin(val)
    {}

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        if (!(keyBegin < keyEnd))
            return;

        //        std::cout << " a map before insertion:" << std::endl;
        //        std::cout << m_valBegin << " ";
        //        for (const auto& it : m_map)
        //            std::cout << "{" << it.first << "," << it.second << "} ";
        //        std::cout << std::endl;
        //        std::cout << " an interval to be inserted:" << std::endl;
        //        std::cout << "{" << keyBegin << "," << keyEnd << "} value:" << val << std::endl;

        // initial insertion
        if (m_map.empty()) {
            m_map.emplace(keyBegin, val);
            m_map.emplace(keyEnd, m_valBegin);
            return;
        }

        // look-up color to the left
        V leftColor = m_valBegin;
        auto itBegin = m_map.upper_bound(keyBegin);

        if (itBegin == m_map.end()) {
            auto rBegin = m_map.rbegin();

            if (!(rBegin->first < keyBegin)) {
                ++rBegin;
            }

            leftColor = rBegin->second;

            ++rBegin;
            itBegin = rBegin.base();
        } else if (itBegin != m_map.begin()) {
            --itBegin;

            if (!(itBegin->first < keyBegin) && !(keyBegin < itBegin->first)) {
                // if
                if (itBegin != m_map.begin()) {
                    --itBegin;
                    leftColor = itBegin->second;
                }
            } else if (itBegin != m_map.begin()) {
                leftColor = itBegin->second;
                --itBegin;
            } else {
                leftColor = itBegin->second;
            }
        }

        // look-up color to the right
        V rightColor = m_valBegin;
        auto itEnd = m_map.upper_bound(keyEnd);
        if (itEnd != m_map.end() && itEnd != m_map.begin()) {
            --itEnd;

            rightColor = itEnd->second;
        }

        // insertion rules
        if (!(leftColor == val)) {
            itBegin = m_map.insert_or_assign(keyBegin, val).first;
        }

        if (!(rightColor == val)) {
            itEnd = m_map.insert_or_assign(keyEnd, rightColor).first;
        }

        if (val == m_valBegin && val == leftColor) {
            itEnd = m_map.begin();
        }

        if (val == m_valBegin && val == rightColor) {
            itEnd = m_map.end();
        }


        if (/*itEnd != m_map.end() && */++itBegin != itEnd) {
//            std::cout << "{" << itBegin->first << "," << itBegin->second << "} " << std::endl;
//            std::cout << "{" << itEnd->first << "," << itEnd->second << "} "  << std::endl;

            m_map.erase(itBegin, itEnd);
        }

        // Fail if the map is not canonical
        auto itPrev = m_map.begin();
        for (auto itNext = ++m_map.begin(); itNext != m_map.end(); ++itNext) {
            std::cout << "prev {" << itPrev->first << "," << itPrev->second << "} "  << std::endl;
            std::cout << "next {" << itNext->first << "," << itNext->second << "} "  << std::endl;

            assert(itPrev->second != itNext->second);
            itPrev = itNext;
        };
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        auto it=m_map.upper_bound(key);
        if(it==m_map.begin()) {
            return m_valBegin;
        } else {
            return (--it)->second;
        }
    }
};

template<typename K, typename V>
void generalCase(const V& initVal, const std::pair<K, K>& key_1, const V& val_1,
                 const std::pair<K, K>& key_2, const V& val_2,
                 const std::tuple<K, V, V>& exp1, const std::tuple<K, V, V>& exp2,
                 const std::tuple<K, V, V>& exp3, const std::tuple<K, V, V>& exp4) {
    interval_map<int, char> iMap(initVal);
    iMap.assign(key_1.first, key_1.second, val_1);

    assert (iMap[key_1.first - 1] == initVal);
    assert (iMap[key_1.first] == val_1);
    assert (iMap[key_1.second - 1] == val_1);
    assert (iMap[key_1.second] == initVal);

    iMap.assign(key_2.first, key_2.second, val_2);

    std::cout << iMap.m_valBegin << " ";
    for (const auto& it : iMap.m_map)
        std::cout << "{" << it.first << "," << it.second << "} ";
    std::cout << std::endl;

    assert (iMap[std::get<0>(exp1) - 1] == std::get<1>(exp1));
    assert (iMap[std::get<0>(exp1)] == std::get<2>(exp1));
    assert (iMap[std::get<0>(exp2) - 1] == std::get<1>(exp2));
    assert (iMap[std::get<0>(exp2)] == std::get<2>(exp2));
    assert (iMap[std::get<0>(exp3) - 1] == std::get<1>(exp3));
    assert (iMap[std::get<0>(exp3)] == std::get<2>(exp3));
    assert (iMap[std::get<0>(exp4) - 1] == std::get<1>(exp4));
    assert (iMap[std::get<0>(exp4)] == std::get<2>(exp4));
}

void generalCaseSuite(char initVal, char val_1, char val_2) {
    generalCase<int, char>(initVal, {20, 40}, val_1, {5, 10}, val_2,
                           {5, initVal, val_2}, {10, val_2, initVal},
                           {20, initVal, val_1}, {40, val_1, initVal});

    // FIXME: wrong for A B B case
    generalCase<int, char>(initVal, {20, 40}, val_1, {10, 20}, val_2,
                           {10, initVal, val_2}, {20, val_2, val_1},
                           {20, val_2, val_1}, {40, val_1, initVal});

    // FIXME: wrong for A B B case
    generalCase<int, char>(initVal, {20, 40}, val_1, {10, 30}, val_2,
                           {10, initVal, val_2}, {20, val_2, val_2},
                           {30, val_2, val_1}, {40, val_1, initVal});

    generalCase<int, char>(initVal, {20, 40}, val_1, {20, 30}, val_2,
                           {20, initVal, val_2}, {20, initVal, val_2},
                           {30, val_2, val_1}, {40, val_1, initVal});

    generalCase<int, char>(initVal, {20, 40}, val_1, {25, 35}, val_2,
                           {20, initVal, val_1}, {25, val_1, val_2},
                           {35, val_2, val_1}, {40, val_1, initVal});

    generalCase<int, char>(initVal, {20, 40}, val_1, {30, 40}, val_2,
                           {20, initVal, val_1}, {30, val_1, val_2},
                           {40, val_2, initVal}, {40, val_2, initVal});

    generalCase<int, char>(initVal, {20, 40}, val_1, {30, 50}, val_2,
                           {20, initVal, val_1}, {30, val_1, val_2},
                           {40, val_2, val_2}, {50, val_2, initVal});

    generalCase<int, char>(initVal, {20, 40}, val_1, {40, 50}, val_2,
                           {20, initVal, val_1}, {40, val_1, val_2},
                           {40, val_1, val_2}, {50, val_2, initVal});

    generalCase<int, char>(initVal, {20, 40}, val_1, {50, 60}, val_2,
                           {20, initVal, val_1}, {40, val_1, initVal},
                           {50, initVal, val_2}, {60, val_2, initVal});
}

int main()
{
    // std::cout <<

    // Test-case for requirement 1:
    {
        std::cout << "Test-case for requirement 1:" << std::endl;

        const char initVal = 'A';
        const int zeroKey = 0,
            minusKey = -123,
            plusKey = 123;

        interval_map<int, char> iMap(initVal);

        assert (iMap[zeroKey] == initVal);
        assert (iMap[minusKey] == initVal);
        assert (iMap[plusKey] == initVal);
    }

    // Test-case for requirement 2-a, 5:
    {
        std::cout << "Test-case for requirement 2-a, 5:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const int beginKey_1 = 0,
            endKey_1 = 15,
            beginKey_2 = -123,
            endKey_2 = 123;

        interval_map<int, char> iMap(initVal);
        iMap.assign(beginKey_1, endKey_1, val_1);

        assert (iMap[beginKey_1 - 1] == initVal);
        assert (iMap[beginKey_1] == val_1);
        assert (iMap[endKey_1 - 1] == val_1);
        assert (iMap[endKey_1] == initVal);

        iMap.assign(beginKey_2, endKey_2, val_2);

        assert (iMap[beginKey_1 - 1] == val_2);
        assert (iMap[beginKey_1] == val_2);
        assert (iMap[endKey_1 - 1] == val_2);
        assert (iMap[endKey_1] == val_2);

        assert (iMap[beginKey_2 - 1] == initVal);
        assert (iMap[beginKey_2] == val_2);
        assert (iMap[endKey_2 - 1] == val_2);
        assert (iMap[endKey_2] == initVal);

    }

    // Test-case for requirement 2-b:
    {
        std::cout << "Test-case for requirement 2-b:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const int beginKey_1 = -123,
            endKey_1 = 123,
            beginKey_2 = 0,
            endKey_2 = 15;

        interval_map<int, char> iMap(initVal);
        iMap.assign(beginKey_1, endKey_1, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[beginKey_1 - 1] == initVal);
        assert (iMap[beginKey_1] == val_1);
        assert (iMap[endKey_1 - 1] == val_1);
        assert (iMap[endKey_1] == initVal);

        iMap.assign(beginKey_2, endKey_2, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[beginKey_1 - 1] == initVal);
        assert (iMap[beginKey_1] == val_1);
        assert (iMap[endKey_1 - 1] == val_1);
        assert (iMap[endKey_1] == initVal);

        assert (iMap[beginKey_2 - 1] ==  val_1);
        assert (iMap[beginKey_2] == val_2);
        assert (iMap[endKey_2 - 1] == val_2);
        assert (iMap[endKey_2] == val_1);
    }

    // Test-case for requirement 3:
    {
        std::cout << "Test-case for requirement 3:" << std::endl;

        const char initVal = 'A',
            newVal = 'B';
        const int beginKey = 123,
            endKey = -123;

        interval_map<int, char> iMap(initVal);
        iMap.assign(beginKey, endKey, newVal);

        assert (iMap[beginKey - 1] == initVal);
        assert (iMap[beginKey] == initVal);
        assert (iMap[endKey - 1] == initVal);
        assert (iMap[endKey] == initVal);

        assert (iMap[beginKey + 1] == initVal);
        assert (iMap[endKey + 1] == initVal);
    }

    // Test-case for complex case 1:
    {
        std::cout << "Test-case for complex case 1:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {-1, 3},
            key_4 {13, 17};

        interval_map<int, char> iMap(initVal);

        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, val_2);
        iMap.assign(key_4.first, key_4.second, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_3.first - 1] == initVal);
        assert (iMap[key_3.first] == val_2);
        assert (iMap[key_1.first - 1] == val_2);
        assert (iMap[key_1.first] == val_2);
        assert (iMap[key_3.second - 1] == val_2);
        assert (iMap[key_3.second] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_4.first - 1] == initVal);
        assert (iMap[key_4.first] == val_1);
        assert (iMap[key_2.first - 1] == val_1);
        assert (iMap[key_2.first] == val_1);
        assert (iMap[key_4.second - 1] == val_1);
        assert (iMap[key_4.second] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);
    }

    // Test-case for complex case 2:
    {
        std::cout << "Test-case for complex case 2:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {1, 4},
            key_4 {16, 18};

        interval_map<int, char> iMap(initVal);
        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, val_2);
        iMap.assign(key_4.first, key_4.second, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_3.first - 1] == val_1);
        assert (iMap[key_3.first] == val_2);
        assert (iMap[key_3.second - 1] == val_2);
        assert (iMap[key_3.second] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_4.first - 1] == val_2);
        assert (iMap[key_4.first] == val_1);
        assert (iMap[key_4.second - 1] == val_1);
        assert (iMap[key_4.second] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);
    }

    // Test-case for complex case 3a:
    {
        std::cout << "Test-case for complex case 3a:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {9, 16};

        interval_map<int, char> iMap(initVal);
        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_3.first - 1] == val_1);
        assert (iMap[key_3.first] == val_2);
        assert (iMap[key_1.second - 1] == val_2);
        assert (iMap[key_1.second] == val_2);
        assert (iMap[key_2.first - 1] == val_2);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_3.second - 1] == val_2);
        assert (iMap[key_3.second] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);
    }

    // Test-case for complex case 3b:
    {
        std::cout << "Test-case for complex case 3b:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {9, 16};

        interval_map<int, char> iMap(initVal);
        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_3.first - 1] == val_1);
        assert (iMap[key_3.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == val_1);
        assert (iMap[key_2.first - 1] == val_1);
        assert (iMap[key_2.first] == val_1);
        assert (iMap[key_3.second - 1] == val_1);
        assert (iMap[key_3.second] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);
    }

    // Test-case for complex case 3c:
    {
        std::cout << "Test-case for complex case 3c:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {9, 16};

        interval_map<int, char> iMap(initVal);
        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, initVal);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_3.first - 1] == val_1);
        assert (iMap[key_3.first] == initVal);
        assert (iMap[key_1.second - 1] == initVal);
        assert (iMap[key_1.second] == initVal);
        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == initVal);
        assert (iMap[key_3.second - 1] == initVal);
        assert (iMap[key_3.second] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);
    }

    // Test-case for complex case interlacing:
    {
        std::cout << "Test-case for complex case interlacing:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {0, 5},
            key_4 {17, 20};

        interval_map<int, char> iMap(initVal);
        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, val_2);
        iMap.assign(key_4.first, key_4.second, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_2);
        assert (iMap[key_3.first - 1] == initVal);
        assert (iMap[key_3.first] == val_2);
        assert (iMap[key_3.second - 1] == val_2);
        assert (iMap[key_3.second] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_4.first - 1] == val_2);
        assert (iMap[key_4.first] == val_1);
        assert (iMap[key_2.second - 1] == val_1);
        assert (iMap[key_2.second] == initVal);
        assert (iMap[key_4.second - 1] == val_1);
        assert (iMap[key_4.second] == initVal);
    }

    // Test-case for complex case interlacing 2:
    {
        std::cout << "Test-case for complex case interlacing 2:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> ins_key_1 {0, 50},
            ins_key_2 {70, 80},
            ins_key_3 {170, 200};

        interval_map<int, char> iMap(initVal);
        iMap.assign(0, 100, val_1);
        iMap.assign(5, 9, initVal);
        iMap.assign(10, 20, val_2);
        iMap.assign(40, 60, initVal);
        iMap.assign(150, 200, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[0 - 1] == initVal);
        assert (iMap[0] == val_1);

        assert (iMap[5 - 1] == val_1);
        assert (iMap[5] == initVal);

        assert (iMap[9 - 1] == initVal);
        assert (iMap[9] == val_1);

        assert (iMap[10 - 1] == val_1);
        assert (iMap[10] == val_2);
        // TBD...

        iMap.assign(ins_key_1.first, ins_key_1.second, val_2);
        iMap.assign(ins_key_2.first, ins_key_2.second, val_1);
        iMap.assign(ins_key_3.first, ins_key_3.second, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        // assert (iMap[key_1.first - 1] == initVal);
        // assert (iMap[key_1.first] == val_2);
        // assert (iMap[key_3.first - 1] == initVal);
        // assert (iMap[key_3.first] == val_2);
        // assert (iMap[key_3.second - 1] == val_2);
        // assert (iMap[key_3.second] == val_1);
        // assert (iMap[key_1.second - 1] == val_1);
        // assert (iMap[key_1.second] == initVal);

        // assert (iMap[key_2.first - 1] == initVal);
        // assert (iMap[key_2.first] == val_2);
        // assert (iMap[key_4.first - 1] == val_2);
        // assert (iMap[key_4.first] == val_1);
        // assert (iMap[key_2.second - 1] == val_1);
        // assert (iMap[key_2.second] == initVal);
        // assert (iMap[key_4.second - 1] == val_1);
        // assert (iMap[key_4.second] == initVal);
    }

    // Test-case for complex case interlacing 3:
    {
        std::cout << "Test-case for complex case interlacing 3:" << std::endl;

        const char initVal = 'A',
            val_1 = 'B',
            val_2 = 'C';
        const std::pair<int, int> key_1 {0, 10},
            key_2 {15, 20},
            key_3 {0, 5},
            key_4 {15, 17};

        interval_map<int, char> iMap(initVal);
        iMap.assign(key_1.first, key_1.second, val_1);
        iMap.assign(key_2.first, key_2.second, val_2);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        assert (iMap[key_1.first - 1] == initVal);
        assert (iMap[key_1.first] == val_1);
        assert (iMap[key_1.second - 1] == val_1);
        assert (iMap[key_1.second] == initVal);

        assert (iMap[key_2.first - 1] == initVal);
        assert (iMap[key_2.first] == val_2);
        assert (iMap[key_2.second - 1] == val_2);
        assert (iMap[key_2.second] == initVal);

        iMap.assign(key_3.first, key_3.second, val_2);
        iMap.assign(key_4.first, key_4.second, val_1);

        std::cout << iMap.m_valBegin << " ";
        for (const auto& it : iMap.m_map)
            std::cout << "{" << it.first << "," << it.second << "} ";
        std::cout << std::endl;

        // assert (iMap[key_1.first - 1] == initVal);
        // assert (iMap[key_1.first] == val_2);
        // assert (iMap[key_3.first - 1] == initVal);
        // assert (iMap[key_3.first] == val_2);
        // assert (iMap[key_3.second - 1] == val_2);
        // assert (iMap[key_3.second] == val_1);
        // assert (iMap[key_1.second - 1] == val_1);
        // assert (iMap[key_1.second] == initVal);

        // assert (iMap[key_2.first - 1] == initVal);
        // assert (iMap[key_2.first] == val_2);
        // assert (iMap[key_4.first - 1] == val_2);
        // assert (iMap[key_4.first] == val_1);
        // assert (iMap[key_2.second - 1] == val_1);
        // assert (iMap[key_2.second] == initVal);
        // assert (iMap[key_4.second - 1] == val_1);
        // assert (iMap[key_4.second] == initVal);
    }

    std::cout << "General test-case 1:" << std::endl;
    generalCaseSuite('A', 'B', 'C');
    std::cout << "General test-case 2:" << std::endl;
    generalCaseSuite('A', 'B', 'B');
    std::cout << "General test-case 3:" << std::endl;
    generalCaseSuite('A', 'B', 'A');

    return 0;
}

/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <tuple>
#include <limits>
#include <vector>
#include <list>

// TODO:
//      1) apply this algorithm to the "find max substring in a string" task
//      2) find a mistake with {-5, -3, -4, -2, 0, 1, 5, 7, 6, 2, 3, 4,-3};
template <class It, class T = typename It::value_type>
std::tuple<It, It, T> findMaxCrossingSubarray(It left, It mid, It right) {
    T leftMaxSum = std::numeric_limits<T>::min();
    It maxLeft = mid;
    T sum = 0;

    for (auto it = mid; it != left; --it) {
        sum += *it;
        if (sum > leftMaxSum) {
            leftMaxSum = sum;
            maxLeft = it;
        }
    }

    T rightMaxSum = std::numeric_limits<T>::min();
    It maxRight = ++mid;
    sum = 0;

    for (auto it = mid; it != right; ++it) {
        sum += *it;
        if (sum > rightMaxSum) {
            rightMaxSum = sum;
            maxRight = it;
        }
    }

    return {maxLeft, maxRight, leftMaxSum + rightMaxSum};
}

template <class It, typename std::enable_if_t<
            std::is_same_v<
                typename std::iterator_traits<It>::iterator_category,
                std::random_access_iterator_tag
            >, bool
         > = true>
It calcMid(It left, It right) {
    return left + std::distance(left, right) / 2;
}

template <class It, typename std::enable_if_t<
            !std::is_same_v<
                typename std::iterator_traits<It>::iterator_category,
                std::random_access_iterator_tag
            >, bool
         > = true>
It calcMid(It left, It right) {
    const auto dist = std::distance(left, right) / 2;
    for (auto i = 0; i < dist; ++i, ++left);
    return left;
}

template <class It, class T = typename It::value_type>
std::tuple<It, It, T> findMaxSubArray(It left, It right) {

    if (left == right)
        return {left, right, *left};

    auto mid = calcMid(left, right);

    auto leftRes = findMaxSubArray(left, mid);
    auto rightRes = findMaxSubArray(++It(mid), right);
    auto midRes = findMaxCrossingSubarray(left, mid, right);

    if (std::get<2>(leftRes) >= std::get<2>(rightRes) and
        std::get<2>(leftRes) >= std::get<2>(midRes))
        return leftRes;
    else if (std::get<2>(rightRes) >= std::get<2>(leftRes) and
        std::get<2>(rightRes) >= std::get<2>(midRes))
        return rightRes;
    else
        return midRes;
}

int main()
{
    {
        std::vector<int> input {-1, 0, 3, 4, 9, 19, -1};

        std::cout << "START" << std::endl;
        auto result = findMaxSubArray(input.begin(), input.end());

        std::cout << "Result {";
        for (auto it = std::get<0>(result); it != std::get<1>(result) + 1; ++it) {
            std::cout << *it << " ";
        }
        std::cout << "}" << std::endl;
    }

    {
        std::list<int> input {-1, 0, -3, 4, -9, 19, 1};

        auto result = findMaxSubArray(input.begin(), input.end());

        std::cout << "Result {";
        auto end = std::get<1>(result); ++end; // TODO: 3) make that the function returns the it after the last element
        for (auto it = std::get<0>(result); it != end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << "}" << std::endl;
    }

    return 0;
}

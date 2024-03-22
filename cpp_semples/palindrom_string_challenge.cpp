#include <iostream>
#include <stack>

class Solution {
public:
    bool isPalindrome(const std::string& s) const {
        if (!s.size())
            return false;

        if (s.size() == 1)
            return true;

        std::stack<std::string::value_type> st;
        auto half = static_cast<std::string::size_type>(s.size() / 2);
        for (std::string::size_type i = 0; i < half; ++i) {
            st.push(s[i]);
        }

        half = s.size() % 2 ? half + 2 : half + 1;
        for (std::string::size_type i = half; i < s.size(); ++i) {
            if (st.top() != s[i]) {
                break;
            }

            st.pop();
        }

        return st.empty();
    }

    bool isPalindrome_v2(const std::string& s) const {
        if (s.empty())
            return false;

        auto it1 = s.begin();
        auto it2 = --s.end();
        while (it1 < it2 && *it1 == *it2) ++it1; --it2;
        return *it1 == *it2;
    }
};



int main() {
    Solution s;
    // std::cout << "isPalindrome v1: " << (s.isPalindrome("ABBA") ? "true" : "false") << std::endl;
    std::cout << "isPalindrome v2_ABCBA: " << (s.isPalindrome_v2("ABCBA") ? "true" : "false") << std::endl;
    std::cout << "isPalindrome v2_ABBA: " << (s.isPalindrome_v2("ABBA") ? "true" : "false") << std::endl;
    std::cout << "isPalindrome v2_ABA: " << (s.isPalindrome_v2("ABA") ? "true" : "false") << std::endl;
    std::cout << "isPalindrome v2_ABCCA: " << (s.isPalindrome_v2("ABCCA") ? "true" : "false") << std::endl;

}

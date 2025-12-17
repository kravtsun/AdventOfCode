#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>

using vint = std::vector<int>;
using int64 = uint64_t;

vint getVint()
{
    std::string line;
    std::getline(std::cin, line);
    std::istringstream lineStream{line};
    std::string s;
    lineStream >> s;
    int num;
    vint result;
    while (lineStream >> num) {
        result.push_back(num);
    }
    return result;
}


int64 getNumber()
{
    std::string line;
    std::getline(std::cin, line);
    std::istringstream lineStream{line};
    std::string s;
    lineStream >> s;
    std::string resultString;
    while (lineStream >> s) {
        resultString += s;
    }

    return std::stoull(resultString);
}

int main() {
    FILE *f = freopen(WORKDIR "input.txt", "r", stdin);
//
//    auto timeNumbers = getVint();
//    auto distanceNumbers = getVint();
//
//    assert(timeNumbers.size() == distanceNumbers.size());
//    int64 result = 1;
//    for (int i = 0; i < timeNumbers.size(); ++i) {
//        auto t = timeNumbers[i];
//        auto d = distanceNumbers[i];
//
//
//        result *= currentResult;
//    }

    auto t = getNumber();
    auto d = getNumber();
    int64 result = 0;
    for (int w = 1; w < t; ++w) {
        const auto ourDistance = (t - w) * w;
        if (ourDistance > d) {
            result++;
        }
    }

    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

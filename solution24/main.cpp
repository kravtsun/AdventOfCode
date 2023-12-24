#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <memory>
#include <numeric>
#include <queue>
#include <set>

using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pdd = std::pair<double, double>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
using vpii = std::vector<pii>;
using spii = std::set<pii>;
using vs = std::vector<std::string>;
using vbool = std::vector<bool>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

template<typename T>
using Point = std::array<T, 3>;

template<typename T>
static std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
    os << p[0] << "," << p[1] << "," << p[2];
    return os;
}


template<typename T>
static std::ostream& operator<<(std::ostream& os, const std::pair<T, T> p) {
    os << p.first << "," << p.second;
    return os;
}

template<typename T>
T det(T a11, T a12, T a21, T a22) {
    return a11 * a22 - a12 * a21;
}

struct Hail {
    Point<int64> p;
    Point<int64> v;

    friend std::istream &operator>>(std::istream &is, Hail &hail) {
        char c;
        is >> hail.p[0] >> c;
        assert(c == ',');
        is >> hail.p[1] >> c;
        assert(c == ',');
        is >> hail.p[2] >> c;
        assert(c == '@');

        is >> hail.v[0] >> c;
        assert(c == ',');
        is >> hail.v[1] >> c;
        assert(c == ',');
        is >> hail.v[2];

        return is;
    }
};

// Kramer's method
std::tuple<bool, pdd> intersect(const Hail& lhs, const Hail& rhs) {
    const auto noAnswer = std::make_tuple(false, pdd{});
    const auto a11 = lhs.v[0], a12 = -rhs.v[0];
    const auto a21 = lhs.v[1], a22 = -rhs.v[1];
    const auto mainDet = det(a11, a12, a21, a22);
    if (mainDet == 0) {
        return noAnswer;
    }

    const auto c1 = rhs.p[0] - lhs.p[0];
    const auto c2 = rhs.p[1] - lhs.p[1];

    const auto lt = static_cast<double>(det(c1, a12, c2, a22)) /
            mainDet;

    const auto rt = static_cast<double>(det(a11, c1, a21, c2)) / mainDet;

    if (lt < 0 || rt < 0) {
        return noAnswer;
    }

    return std::make_tuple(true, pdd(lhs.p[0] + lhs.v[0] * lt, lhs.p[1] + lhs.v[1] * lt));
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
//    std::ifstream fin{WORKDIR "input_sample.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::vector<Hail> hails;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::istringstream is{line};
        Hail hail;
        is >> hail;
        hails.push_back(hail);
    }

//    int i = 0;
//    int j = 3;

    const int64 TEST_AREA_MIN = 200000000000000LL;
    const int64 TEST_AREA_MAX = 400000000000000LL;
//    const int64 TEST_AREA_MIN = 7;
//    const int64 TEST_AREA_MAX = 27;

    int result = 0;
    for (int i = 0; i < hails.size(); ++i) {
        for (int j = i + 1; j < hails.size(); ++j) {
            auto [hasIntersection, intersectionPoint] = intersect(hails[i], hails[j]);
#if 0
            std::cout << "Hailstone A: " << hails[i].p << " -> " << hails[i].v << std::endl;
            std::cout << "Hailstone B: " << hails[j].p << " -> " << hails[j].v << std::endl;
            if (!hasIntersection) {
                std::cout << " => no solution" << std::endl;
            } else {
                std::cout << " => " << intersectionPoint << std::endl;
            }
#endif

            if (intersectionPoint.first >= TEST_AREA_MIN && intersectionPoint.first <= TEST_AREA_MAX &&
                intersectionPoint.second >= TEST_AREA_MIN && intersectionPoint.second <= TEST_AREA_MAX &&
                hasIntersection) {
                result++;
            }
        }
    }

    std::cout << result << std::endl;

    return 0;
}
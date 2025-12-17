#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include <type_traits>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <cmath>

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
using vdouble = std::vector<double>;
using vvdouble = std::vector<vdouble>;

template<typename T>
using Point = std::array<T, 3>;

template<typename T>
static std::ostream &operator<<(std::ostream &os, const Point<T> &p) {
    os << p[0] << "," << p[1] << "," << p[2];
    return os;
}


template<typename T>
static std::ostream &operator<<(std::ostream &os, const std::pair<T, T> p) {
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
std::tuple<bool, pdd> intersect(const Hail &lhs, const Hail &rhs) {
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

static int64 star1(const std::vector<Hail> &hails) {
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
    return result;
}

enum Equation {
    YZ, ZX, XY
};

// p0x, p0y, p0z, v0x, v0y, v0z, C
using Coefficients = std::array<int64, 7>;

Coefficients coefficients(const Hail &hail, Equation equation) {
    const auto v1x = hail.v[0];
    const auto v1y = hail.v[1];
    const auto v1z = hail.v[2];

    const auto p1x = hail.p[0];
    const auto p1y = hail.p[1];
    const auto p1z = hail.p[2];

    switch (equation) {
        case YZ: return Coefficients{0, -v1z, v1y, 0, p1z, -p1y, - p1y * v1z + p1z * v1y};
        case ZX: return Coefficients{v1z, 0, -v1x, -p1z, 0, p1x, - p1z * v1x + p1x * v1z};
        case XY: return Coefficients{-v1y, v1x, 0, p1y, -p1x, 0, - p1x * v1y + p1y * v1x};
    }
    assert(false);
}

template<typename T>
// https://e-maxx.ru/algo/linear_systems_gauss
std::vector<T> gauss(std::vector<std::vector<T>> a) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;

    static constexpr bool isIntegral = std::is_integral_v<T>;

    const auto EPS = 1e-7;

    vint where(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row; i < n; ++i)
            if (std::abs(a[i][col]) > std::abs(a[sel][col]))
                sel = i;
        if (std::abs(a[sel][col]) < EPS)
            continue;
        for (int i = col; i <= m; ++i)
            std::swap(a[sel][i], a[row][i]);
        where[col] = row;

        for (int i = 0; i < n; ++i)
            if (i != row) {
                if constexpr (isIntegral) {
                    assert(a[i][col] % a[row][col] == 0);
                }
                auto c = a[i][col] / a[row][col]; // double
                for (int j = col; j <= m; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        ++row;
    }

    std::vector<T> ans(m, 0);
    for (int i = 0; i < m; ++i) {
        if (where[i] != -1) {
            if constexpr (isIntegral)
                assert(a[where[i]][m] % a[where[i]][i] == 0);
            ans[i] = a[where[i]][m] / a[where[i]][i];
        }
    }

    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < m; ++j)
            sum += ans[j] * a[i][j];
        if (std::abs(sum - a[i][m]) > EPS) {
            assert(!isIntegral);
//            return 0;
        }
    }

    for (int i = 0; i < m; ++i) {
        if (where[i] == -1) {
            assert(!isIntegral);
//            return INF;
        }
    }

    return ans;
//    return 1;
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

//    std::cout << star1(hails) << std::endl;

    const int NUNKNOWN = 6;
    std::array<Coefficients, NUNKNOWN> a;
    int aIndex = 0;

    for (int i = 0; i < 2; ++i) {
        const std::array<Equation, 3> equations{Equation::YZ, Equation::ZX, Equation::XY};

        for (auto equation: equations) {
            int j = i + 2;
            auto icoeffs = coefficients(hails[i], equation);
            auto jcoeffs = coefficients(hails[j], equation);
            Coefficients diffCoeffs;

            for (int k = 0; k < diffCoeffs.size(); ++k) {
                diffCoeffs[k] = icoeffs[k] - jcoeffs[k];
            }

            assert(aIndex < 6);
            a[aIndex++] = diffCoeffs;
        }
    }

    using T = long double;
    std::vector<std::vector<T>> aVector(NUNKNOWN, std::vector<T>(NUNKNOWN + 1));
    assert(aVector.size() == a.size());
    assert(aVector[0].size() == a[0].size());

    for (int i = 0; i < NUNKNOWN; ++i) {
        for (int j = 0; j < NUNKNOWN + 1; ++j) {
            aVector[i][j] = a[i][j];
            std::cout << a[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    auto ans = gauss<T>(aVector);

    std::vector<int64> ansInteger(ans.size());

    for (int i = 0; i < ans.size(); ++i) {
        ansInteger[i] = static_cast<int64>(std::round(ans[i]));
        std::cout << static_cast<int64>(std::round(ans[i])) << std::endl;
    }

    std::cout << ansInteger[0] + ansInteger[1] + ansInteger[2] << std::endl;

    return 0;
}
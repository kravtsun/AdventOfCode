// This software was partially written using Suggestions from GitHub Copilot.
#if _MSC_VER
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>
#include <array>
#include <queue>

#include <utility>
#else
#include <bits/stdc++.h>
#endif

#include <utility>

using namespace std;

#define all(x) (x).begin(), (x).end()
#define A auto

using int64 = long long;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vvint = vector<vint>;
using vdouble = vector<double>;
using vvdouble = vector<vdouble>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii = vector<pii>;
using vpii64 = vector<pii64>;
using vbool = vector<bool>;

static auto INF = std::numeric_limits<int>::max();

#if _MSVC_VER
static const std::string EXAMPLE_INPUT_FILE = "example_input.txt";
static const std::string INPUT_FILE = "C:\\Users\\Andrei\\Projects\\AdventOfCode\\2025\\solution10\\input.txt";
#else
static const std::string EXAMPLE_INPUT1_FILE = "example_input1.txt";
static const std::string EXAMPLE_INPUT2_FILE = "example_input2.txt";
static const std::string EXAMPLE_INPUT3_FILE = "example_input3.txt";
static const std::string INPUT_FILE = "input.txt";
#endif

template<typename T = int64>
T from_string(const std::string &s) {
    std::istringstream is(s);
    T x;
    is >> x;
    return x;
}

template<typename T>
T gcd(T a, T b) {
    return b ? gcd(b, a % b) : a;
}

template<typename T=int64>
T lcm(T a, T b) {
    return a / gcd(a, b) * b;
}

static const std::vector<std::string> numbers = {
        "zero",
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
};

static auto readLines(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

template<typename T>
T sqr(T x) { return x * x; }

template<typename T = int, int N = 2>
struct PointTmpl {
    static_assert(N > 1, "Number of coordinates should be bigger than 1");
    std::array<T, N> coords;

    explicit PointTmpl(const std::array<T, N> &arr)
            : coords(arr) {
    }

    PointTmpl(T y, T x) requires (N == 2)
            : coords({y, x}) {
    }

    PointTmpl(T x, T y, T z) requires (N == 3)
            : coords({x, y, z}) {
    }

    PointTmpl()
            : coords({0}) {}

    PointTmpl(const PointTmpl &rhs) = default;

    PointTmpl(PointTmpl &&rhs) = default;

    PointTmpl &operator=(const PointTmpl &rhs) = default;

    PointTmpl &operator=(PointTmpl &&rhs) = default;

    [[nodiscard]] const T &x() const { return coords[0]; }

    T &x() { return coords[0]; }

    [[nodiscard]] const T &y() const { return coords[1]; }

    T &y() { return coords[1]; }

    [[nodiscard]] const T &z() const requires(N >= 3) {
        return coords[2];
    }

    T &z() requires(N >= 2) { return coords[2]; }

    [[nodiscard]] auto sqr() const {
        static_assert(std::is_integral_v<T>);
        int64 res = 0;
        for (int i = 0; i < N; ++i) {
            res += ::sqr(coords[i]);
        }
        return res;
    }

    [[nodiscard]] auto distSquared(const PointTmpl &rhs) const {
        static_assert(std::is_integral_v<T>);
        int64 res = 0;
        for (int i = 0; i < N; ++i) {
            res += ::sqr(coords[i] - rhs.coords[i]);
        }
        return res;
    }

    [[nodiscard]] double dist(const PointTmpl &rhs) const {
        return sqrt(static_cast<double>(distSquared(rhs)));
    }

    T &operator[](int i) {
        if (i >= N) {
            throw std::runtime_error("operator is out of range for coordinates, i=" +
                                     std::to_string(i) + "/" + std::to_string(N));
        }
        return coords[i];
    }

    const T &operator[](int i) const {
        if (i >= N) {
            throw std::runtime_error("operator is out of range for coordinates, i=" +
                                     std::to_string(i) + "/" + std::to_string(N));
        }
        return coords[i];
    }

    template<typename U, int M>
    friend std::ostream &operator<<(std::ostream &os, const PointTmpl<U, M> &p);

    [[nodiscard]] auto left() const requires (N == 2) {
        std::array<T, N> newCoords = coords;
        newCoords[1]--;
        return PointTmpl<T, N>(newCoords);
    }

    [[nodiscard]] auto right() const requires (N == 2) {
        std::array<T, N> newCoords = coords;
        newCoords[1]++;
        return PointTmpl<T, N>(newCoords);
    }

    [[nodiscard]] auto up() const requires (N == 2) {
        std::array<T, N> newCoords = coords;
        newCoords[0]--;
        return PointTmpl<T, N>(newCoords);
    }

    [[nodiscard]] auto down() const requires (N == 2) {
        std::array<T, N> newCoords = coords;
        newCoords[0]++;
        return PointTmpl<T, N>(newCoords);
    }

    PointTmpl<T, N> min(const PointTmpl<T, N> &rhs) {
        PointTmpl<T, N> res(coords);
        for (int i = 0; i < N; ++i) {
            res[i] = std::min(res[i], rhs[i]);
        }
        return res;
    }

    PointTmpl<T, N> max(const PointTmpl<T, N> &rhs) {
        PointTmpl<T, N> res(coords);
        for (int i = 0; i < N; ++i) {
            res[i] = std::max(res[i], rhs[i]);
        }
        return res;
    }
};

template<typename T, int N>
std::ostream &operator<<(std::ostream &os, const PointTmpl<T, N> &p) {
    for (int i = 0; i < N; ++i) {
        if (i) {
            os << ", ";
        }
        os << p[i];
    }
    return os;
}

template<typename T, int N>
std::string to_string(const PointTmpl<T, N> &p) {
    std::ostringstream os;
    os << p;
    return os.str();
}

template<typename T, int N>
PointTmpl<T, N> operator+(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        res[i] = lhs[i] + rhs[i];
    }
}

template<typename T, int N>
PointTmpl<T, N> operator*(const PointTmpl<T, N> &lhs, const T x) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        res[i] = lhs[i] * x;
    }
}

template<typename T, int N>
PointTmpl<T, N> operator<(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) return lhs[i] < rhs[i];
    }
    return false;
}

template<typename T, int N>
PointTmpl<T, N> operator==(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    PointTmpl<T, N> res;
    for (int i = 0; i < N; ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}


template<typename T, int N>
PointTmpl<T, N> operator!=(const PointTmpl<T, N> &lhs, const PointTmpl<T, N> &rhs) {
    return !(lhs == rhs);
}

using Point = PointTmpl<int64, 2>;
using Point3 = PointTmpl<int64, 3>;

static bool isGoodPoint(const Point &p, int n, int m) {
    return p.y() >= 0 && p.y() < n && p.x() >= 0 && p.x() < m;
}

static int char2digit(char c) {
    if (isalpha(c)) {
        assert(tolower(c) == c || toupper(c) == c);
        if (tolower(c) == c) {
            return 10 + c - 'a';
        } else {
            return 10 + c - 'A';
        }
    }
    return c - '0';
}

template<typename T>
static auto mergeRanges(vector<pair<T, T>> ranges) {
    vector<pair<T, T>> newRanges;
    if (ranges.empty()) return newRanges;

    sort(ranges.begin(), ranges.end());

    auto curRange = ranges.front();
    for (int i = 1; i < ranges.size(); ++i) {
        auto r = ranges[i];
        if (r.second <= curRange.second) {
            continue;
        }
        if (r.first <= curRange.second) {
            curRange.second = r.second;
        } else {
            assert(r.first > curRange.second);
            newRanges.push_back(curRange);
            curRange = r;
        }
    }
    newRanges.push_back(curRange);
    return newRanges;
}

struct Field {
    explicit Field(const std::string &filepath)
            : lines(readLines(filepath)) {
        checkLines();
    }

    explicit Field(std::vector<std::string> lines)
            : lines(std::move(lines)) {
        checkLines();
    }

    [[nodiscard]] bool isGoodPoint(const Point &p) const {
        const auto n = height();
        const auto m = width();
        return p.y() >= 0 && p.y() < n && p.x() >= 0 && p.x() < m;
    }

    char &operator[](const Point &p) {
        if (!isGoodPoint(p)) {
            throw std::runtime_error("Bad point: " + to_string(p));
        }
        return lines[p.y()][p.x()];
    }

    const char &operator[](const Point &p) const {
        if (!isGoodPoint(p)) {
            throw std::runtime_error("Bad point: " + to_string(p));
        }
        return lines[p.y()][p.x()];
    }

    [[nodiscard]] int height() const {
        return lines.size();
    }

    [[nodiscard]] int width() const {
        return lines[0].size();
    }

    [[nodiscard]] auto symbolLoc(char symbol) const {
        for (int i = 0; i < height(); ++i) {
            for (int j = 0; j < width(); ++j) {
                if (lines[i][j] == symbol) {
                    return Point{i, j};
                }
            }
        }
        throw std::runtime_error("Not found symbol: " + std::string(1, symbol));
//        return {-1, -1};
    }

private:
    std::vector<std::string> lines;

    void checkLines() const {
        assert(!lines.empty());
        auto w = lines[0].size();
        for (const auto &l: lines) {
            if (l.size() != w) {
                throw std::runtime_error("Inconsistent line: \n" + l);
            }
        }
    }
};

static bool bitGet(int curSwitch, int i) {
    return curSwitch & (1 << i);
}

static int bitSet(int curSwitch, int i) {
    return curSwitch | (1 << i);
}

void star1()
{
    std::ifstream fin{EXAMPLE_INPUT1_FILE};
//    std::ifstream fin{INPUT_FILE};
//    assert(fin.is_open());

    map<string, vector<string>> g;

    string line;
    while (std::getline(fin, line)) {
        istringstream is(line);
        string from;
        is >> from;
        assert(from.back() == ':');
        from.pop_back();
        string to;
        while (is >> to) {
            g[from].push_back(to);
        }
    }

    queue<string> q;
    q.push("you");
    map<string, int> d;
    d["you"] = 1;
    map<string, bool> used;
    used["you"] = true;

//    std::function<void(string)> DFS;
//    DFS = [&](string from) {
//        used[from] = true;
//        for (auto to : g[from]) {
//            d[to] += d[from];
//            if (!used[to]) {
//                DFS(to);
//            }
//        }
//    };
//    DFS("you");

    while (!q.empty()) {
        auto from = q.front();
        q.pop();
        for (auto to : g[from]) {
            d[to] += d[from];
            if (!used[to]) {
                used[to] = true;
                q.push(to);
            }
        }
    }

    for (auto [v, dist] : d) {
        cout << v << " " << dist <<endl;
    }

//    cout << d["out"] << endl;
}

string cycle_st, cycle_end;

map<string, char> cl;
map<string, string> p;

int main() {
//    star1();
//    return 0;

//    std::ifstream fin{EXAMPLE_INPUT2_FILE};
    std::ifstream fin{INPUT_FILE};
//    assert(fin.is_open());

    map<string, vector<string>> g;

    string line;
    while (std::getline(fin, line)) {
        istringstream is(line);
        string from;
        is >> from;
        assert(from.back() == ':');
        from.pop_back();
        string to;
        while (is >> to) {
            g[from].push_back(to);
        }
    }

    std::function<int64(string, string)> DFS;
    map<string, int64> memo;
    DFS = [&g, &DFS, &memo](string v, string dest) {
        if (v == dest) {
            return 1LL;
        }
        if (memo.count(v)) {
            return memo[v];
        }
        int64 res = 0;
        for (const auto& to : g[v]) {
            res += DFS(to, dest);
        }
        return memo[v] = res;
    };

    auto numPaths = [&g, &DFS, &memo](string start, string finish) {
        memo.clear();
        return DFS(start, finish);
        queue<string> q;
        q.push(start);

        map<string, int64> d;
        d[start] = 1;

//        map<string, pair<int, bool>> d;
//    d["svr"] = pair(1, false);
        map<string, bool> used;
        used[start] = true;

//    std::function<void(string)> DFS;
//    DFS = [&](string from) {
//        used[from] = true;
//        for (auto to : g[from]) {
//            d[to] += d[from];
//            if (!used[to]) {
//                DFS(to);
//            }
//        }
//    };
//    DFS("you");

        while (!q.empty()) {
            auto from = q.front();
            q.pop();
            for (const auto& to : g[from]) {
                d[to] += d[from];
                if (!used[to]) {
                    used[to] = true;
                    q.push(to);
                }
            }
        }

//        for (auto [v, dist] : d) {
//            cout << v << " " << dist <<endl;
//        }

        return d[finish];
    };

    auto START = "svr";
    auto FINISH = "out";
    auto INTERMEDIATE1 = "fft";
    auto INTERMEDIATE2 = "dac";

    const auto starti1 = numPaths(START, INTERMEDIATE1);
    const auto i1i2 = numPaths(INTERMEDIATE1, INTERMEDIATE2);
    const auto i2finish = numPaths(INTERMEDIATE2, FINISH);
    const auto finishi2 = numPaths(FINISH, INTERMEDIATE2);

    const auto starti2 = numPaths(START, INTERMEDIATE2);
    const auto i2i1 = numPaths(INTERMEDIATE2, INTERMEDIATE1);
    const auto i1finish = numPaths(INTERMEDIATE1, FINISH);
    const auto finishi1 = numPaths(FINISH, INTERMEDIATE1);
    cout << starti1 * i1i2 * i2finish // << endl;
    + starti2 * i2i1 * i1finish << endl;

    return 0;
}

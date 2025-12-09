// This software was partially written using Suggestions from GitHub Copilot.
#include <bits/stdc++.h>

#define all(x) x.begin(), x.end()

using int64 = long long;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vint64 = std::vector<int64>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;

template<typename T = int64>
T from_string(const std::string &s) {
    std::istringstream is(s);
    T x;
    is >> x;
    return x;
}

template<typename T>
T gcd(T a, T b) {
    return b ? gcd(b, a%b) : a;
}

template<typename T=int64> T lcm(T a, T b) {
    return a / gcd(a, b) * b;
}

//struct Vertex {
//    Vertex() {};
//};

// BFS
//std::vector<std::vector<int>> g;
//std::vector<bool> used(n, false);
//std::queue<Vertex> q;
//q.push(v);
//used[v] = true;
//
//while (!q.empty()) {
//auto v = q.front();
//q.pop();
//for (auto to : g[v].tos) {
//if (!used[to]) {
//q.push(to);
//used[to] = true;
//}
//}
//}

//
//std::ifstream fin{WORKDIR "input.txt"};
//assert(fin.is_open());
//std::cin.rdbuf(fin.rdbuf());

static const std::vector<std::string> numbers = {
    "zero",
    "one","two","three","four","five","six","seven","eight","nine",
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

using Point = std::pair<int, int>;

static auto addPoint(const Point &lhs, const Point &rhs) {
    return Point{lhs.first + rhs.first, lhs.second + rhs.second};
}

static bool isGoodPoint(const Point &p, int n, int m) {
    return p.first >= 0 && p.first < n && p.second >= 0 && p.second < m;
}

using namespace std;

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

int main() {
//    auto lines = readLines("example_input.txt");
    auto lines = readLines("input.txt");

    vector<pii64> ranges;
    int i0;
    for (i0 = 0; i0 < lines.size(); ++i0) {
        const auto l = lines[i0];
        if (l.empty()) break;
        istringstream is(l);
        int64 a, b;
        char c;
        is >> a >> c >> b;
        assert(c == '-');
        ranges.emplace_back(a, b);
    }

//    sort(ranges.begin(), ranges.end());
    ranges = mergeRanges(ranges);
    int64 res = 0;
    for (auto r : ranges) {
        res += r.second - r.first + 1;
    }

//    auto curRange = ranges.front();
//    for (int i = 1; i < ranges.size(); ++i) {
//        auto r = ranges[i];
//        if (r.second <= curRange.second) {
//            continue;
//        }
//        if (r.first <= curRange.second) {
//            curRange.second = r.second;
//        } else {
//            assert(r.first > curRange.second);
//            res += curRange.second - curRange.first + 1;
//            curRange = r;
//        }
//    }
//    res += curRange.second - curRange.first + 1;

    cout << res << endl;

//    int res = 0;
//    for (int j = i0 + 1; j < lines.size(); ++j) {
//        auto l = lines[j];
//        istringstream is(l);
//        int64 num;
//        is >> num;
//
//        bool fl = false;
//        for (auto [a, b] : ranges) {
//            if (num >= a && num <= b) {
//                fl = true;
//                break;
//            }
//        }
//        if (fl) {
//            res++;
//        }
//    }
//
//    cout << res <<endl;
    return 0;
}

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
//    std::ifstream fin{"input.txt"};
//    std::ifstream fin{"example_input.txt"};
//    assert(fin.is_open());
//    std::cin.rdbuf(fin.rdbuf());

//    auto lines = readLines("example_input.txt");
    auto lines = readLines("input.txt");

    using pic = pair<int, char>;
    vector<pic> signs;
    {
        auto signsLine = lines.back();
        char c;
        for (int i = 0; i < signsLine.size(); ++i) {
            if (signsLine[i] != ' ') {
                signs.emplace_back(i, signsLine[i]);
            }
        }
        lines.pop_back();
    }

    int64 res = 0;

    for (int is = 0; is < signs.size(); ++is) {
        auto [signPos, sign] = signs[is];
        std::function<int64(int64, int64)> op;
        if (sign == '+') {
            op = [](int64 a, int64 b) { return a + b; };
        } else if (sign == '*') {
            op = [](int64 a, int64 b) { return a * b; };
        } else {
            assert(false);
        }

        int prevPos = is + 1 == signs.size() ? lines[0].size() : signs[is + 1].first;
        int64 cur_res = -1;
        for (int j = prevPos - 1; j >= signPos; --j) {
            int64 cur_num = 0;
            for (int i = 0; i < lines.size(); ++i) {
                if (lines[i][j] != ' ') {
                    cur_num *= 10;
                    cur_num += char2digit(lines[i][j]);
                }
            }
            std::cout << "cur_num=" << cur_num << endl;
            if (cur_num == 0) continue;
            if (cur_res == -1) {
                cur_res = cur_num;
            } else {
                cur_res = op(cur_res, cur_num);
            }
        }
        std::cout << "\tcur_res=" << cur_res << endl;
        res += cur_res;
    }

    cout << res << endl;
    return 0;
}

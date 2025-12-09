// This software was partially written using Suggestions from GitHub Copilot.
#include <bits/stdc++.h>

#include <utility>

using namespace std;

#define all(x) x.begin(), x.end()
#define A auto

using int64 = long long;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vint64 = std::vector<int64>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii = vector<pii>;
using vpii64 = vector<pii64>;

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

using Point = std::pair<int, int>;
std::string to_string(const Point &p) {
    return "(" + std::to_string(p.first) + ", " + std::to_string(p.second) + ")";
}

static auto getPointLeft(const Point &lhs) {
    return Point{lhs.first, lhs.second - 1};
}

static auto getPointRight(const Point &lhs) {
    return Point{lhs.first, lhs.second + 1};
}

static auto getPointUp(const Point &lhs) {
    return Point{lhs.first - 1, lhs.second};
}

static auto getPointDown(const Point &lhs) {
    return Point{lhs.first + 1, lhs.second};
}

static auto addPoint(const Point &lhs, const Point &rhs) {
    return Point{lhs.first + rhs.first, lhs.second + rhs.second};
}

static bool isGoodPoint(const Point &p, int n, int m) {
    return p.first >= 0 && p.first < n && p.second >= 0 && p.second < m;
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
        return p.first >= 0 && p.first < n && p.second >= 0 && p.second < m;
    }

    char &operator[](const Point &p) {
        if (!isGoodPoint(p)) {
            throw std::runtime_error("Bad point: " + to_string(p));
        }
        return lines[p.first][p.second];
    }

    const char &operator[](const Point &p) const {
        return lines[p.first][p.second];
    }

    int height() const {
        return lines.size();
    }

    int width() const {
        return lines[0].size();
    }

    auto symbolLoc(char symbol) const {
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

static int64 star(const std::string &filepath, bool isSecondStar) {
    auto lines = readLines(filepath);

    Point startPos;
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < lines[0].size(); ++j) {
            if (lines[i][j] == 'S') {
                startPos = {i, j};
                break;
            }
        }
    }

    queue<Point> q;
    map<Point, bool> used;
    map<Point, int64> d;
    q.push(startPos);
    used[startPos] = true;
    d[startPos] = 1;

    const auto n = lines.size();
    const auto m = lines[0].size();
    const auto try_go = [&](const Point &from, const Point &pNext) {
        if (isGoodPoint(pNext, n, m)) {
            d[pNext] += d[from];
            if (!used[pNext]) {
                used[pNext] = true;
                q.push(pNext);
            }
        }
    };

    int64 res = 0;
    while (!q.empty()) {
//        for (auto l : lines) {
//            cout << l << endl;
//        }
//        cout << endl;
        Point p = q.front();
        q.pop();
        lines[p.first][p.second] = '|';
        auto pDown = getPointDown(p);
        if (!isGoodPoint(pDown, n, m)) continue;

        if (lines[pDown.first][pDown.second] == '^') {
            res += !isSecondStar;
            auto pLeft = getPointLeft(pDown);
            auto pRight = getPointRight(pDown);
            try_go(p, pLeft);
            try_go(p, pRight);
        } else {
            try_go(p, pDown);
        }
    }
    if (isSecondStar) {
        for (int i = 0; i < m; ++i) {
            res += d[Point{n - 1, i}];
        }
    }
    return res;
}

static auto star1(const std::string &filepath) {
    return star(filepath, false);
}

static auto star2(const std::string &filepath) {
    return star(filepath, true);
}

int main() {
    cout << star1("example_input.txt") << endl;
    cout << star1("input.txt") << endl;
    cout << star2("example_input.txt") << endl;
    cout << star2("input.txt") << endl;

    return 0;
}

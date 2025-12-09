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

template<typename T>
static auto INF = std::numeric_limits<T>::max();

static const std::string EXAMPLE_INPUT_FILE = "example_input.txt";
static const std::string INPUT_FILE = "input.txt";

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

    template<typename U, int M>
    friend PointTmpl<U, M> operator+(const PointTmpl<U, M> &lhs, const PointTmpl<U, M> &rhs);
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

using Point = PointTmpl<int, 2>;
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

static auto star1() {
    //    std::ifstream fin{EXAMPLE_INPUT_FILE};
    std::ifstream fin{INPUT_FILE};
    string line;

    vpii mas;
    while (std::getline(fin, line)) {
        istringstream is(line);
        int a, b;
        char c;
        is >> a >> c >> b;
        mas.emplace_back(a, b);
    }

    int64 res = 0;
    for (int i = 0; i < mas.size(); ++i) {
        for (int j = i + 1; j < mas.size(); ++j) {
            int64 curArea = (int64)(mas[i].first - mas[j].first + 1) * (mas[i].second - mas[j].second + 1);
            if (res < curArea) {
                res = curArea;
            }
        }
    }

    cout << res << endl;
    return 0;

}


const double EPS = 1E-9;

struct pt {
    pt(double x = 0, double y = 0) : x(x), y(y) {}
    double x, y;

    bool operator< (const pt & p) const {
        return x < p.x-EPS || abs(x-p.x) < EPS && y < p.y - EPS;
    }
};

bool operator==(const pt &a, const pt &b) {
    return fabs(a.x - b.x) < EPS && fabs(a.y - b.y) < EPS;
}


bool operator!=(const pt &a, const pt &b) {
    return !(a == b);
}

struct line {
    double a, b, c;

    line() {}
    line (pt p, pt q) {
        a = p.y - q.y;
        b = q.x - p.x;
        c = - a * p.x - b * p.y;
        norm();
    }

    void norm() {
        double z = sqrt (a*a + b*b);
        if (abs(z) > EPS)
            a /= z,  b /= z,  c /= z;
    }

    double dist (pt p) const {
        return a * p.x + b * p.y + c;
    }
};

#define det(a,b,c,d)  (a*d-b*c)

inline bool betw (double l, double r, double x) {
    return min(l,r) <= x + EPS && x <= max(l,r) + EPS;
}

inline bool intersect_1d (double a, double b, double c, double d) {
    if (a > b)  swap (a, b);
    if (c > d)  swap (c, d);
    return max (a, c) <= min (b, d) + EPS;
}

bool intersect (pt a, pt b, pt c, pt d, pt & left, pt & right) {
    if (! intersect_1d (a.x, b.x, c.x, d.x) || ! intersect_1d (a.y, b.y, c.y, d.y))
        return false;
    line m (a, b);
    line n (c, d);
    double zn = det (m.a, m.b, n.a, n.b);
    if (abs (zn) < EPS) {
        if (abs (m.dist (c)) > EPS || abs (n.dist (a)) > EPS)
            return false;
        if (b < a)  swap (a, b);
        if (d < c)  swap (c, d);
        left = max (a, c);
        right = min (b, d);
        return true;
    }
    else {
        left.x = right.x = - det (m.c, m.b, n.c, n.b) / zn;
        left.y = right.y = - det (m.a, m.c, n.a, n.c) / zn;
        return betw (a.x, b.x, left.x)
               && betw (a.y, b.y, left.y)
               && betw (c.x, d.x, left.x)
               && betw (c.y, d.y, left.y);
    }
}

map<int, set<int>> lines;
set<pii> points;
int main() {
//    std::ifstream fin{EXAMPLE_INPUT_FILE};
    std::ifstream fin{INPUT_FILE};
    string line;

    vector<pair<double, double>> mas;
    while (std::getline(fin, line)) {
        istringstream is(line);
        int a, b;
        char c;
        is >> a >> c >> b;
        mas.emplace_back(a, b);
        lines[a].insert(b);
        lines[b].insert(a);
        points.emplace(a, b);
    }
    const auto n = mas.size();

    auto masBigger = mas;
    for (int i = 0; i < n; ++i) {
        masBigger.emplace_back((mas[i].first + mas[(i + 1) % n].first) / 2., (mas[i].second + mas[(i + 1) % n].second) / 2.);
    }

    int64 res = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            bool isGood = true;
//            pt b{static_cast<double>(mas[j].first), static_cast<double>(mas[j].second)};
//            pt a{static_cast<double>(mas[i].first), static_cast<double>(mas[i].second)};
//            vector<pt> rectanglePoints;
//            rectanglePoints.push_back(a);
//            rectanglePoints.push_back(pt(a.x, b.y));
//            rectanglePoints.push_back(b);
//            rectanglePoints.push_back(pt(b.x, a.y));
//
//            auto checkGood = [&mas, &rectanglePoints](int k, int kNext) {
//                for (int l = 0; l < 4; ++l) {
//                    auto a = rectanglePoints[l];
//                    auto b = rectanglePoints[(l + 1) % 4];
//                    pt c(mas[k].first, mas[k].second);
//                    pt d(mas[kNext].first, mas[kNext].second);
////                    d.x += 100000 * (d.x - c.x);
////                    d.y += 100000 * (d.y - c.y);
//                    pt left, right;
//                    bool doIntersect = intersect(a, b, c, d, left, right);
//                    if (!doIntersect) return true;
//                    if (left == right && left != a && left != b) {
//                        return false;
//                    }
//                }
//
//                return true;
//            };

//            for (int k = j + 1; k != j; k = (k + 1) % n) {
//                auto kNext = (k + 1) % n;
//                if (!checkGood(k, kNext)) {
//                    isGood = false;
//                    break;
//                }
//            }
//
//            for (int k = (j - 1 + n) % n; k != j; k = (k - 1 + n) % n) {
//                auto kNext = (k - 1 + n) % n;
//                if (!checkGood(k, kNext)) {
//                    isGood = false;
//                    break;
//                }
//            }

            int64 curArea = (int64)(abs(mas[i].first - mas[j].first) + 1) * (abs(mas[i].second - mas[j].second) + 1);
            pii pMin{std::min(mas[i].first, mas[j].first), std::min(mas[i].second, mas[j].second)};
            pii pMax{std::max(mas[i].first, mas[j].first), std::max(mas[i].second, mas[j].second)};
//            if (curArea == 40) {
//                cout << endl;
//            }

//            for (int k = 0; k < n; k++) {
            for (auto p : masBigger) {
                if (p.first > pMin.first && p.first < pMax.first &&
                    p.second > pMin.second && p.second < pMax.second) {
                    isGood = false;
                    break;
                }
            }

            if (isGood) {
//                int64 curArea = (int64)(abs(mas[i].first - mas[j].first) + 1) * (abs(mas[i].second - mas[j].second) + 1);
                res = std::max(res, curArea);
            }
        }
    }

    cout << res << endl;
    return 0;
}

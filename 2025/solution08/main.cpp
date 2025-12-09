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

    [[nodiscard]] const T &z() const requires(N >= 2) {
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

// set k to infinity for the second star.
static auto star(const std::string &filepath, int k = INF<int>) {
    const auto isSecondStar = k == INF<int>;
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    vector<Point3> points;

    string line;
    while (std::getline(cin, line)) {
        istringstream is(line);
        char c;
        int x, y, z;
        is >> x >> c >> y >> c >> z;
        points.emplace_back(x, y, z);
    }

    vint dsu(points.size());
    std::iota(dsu.begin(), dsu.end(), 0);

    std::function<int(int)> getCC;
    getCC = [&dsu, &getCC](int i) {
        if (dsu[i] == i) return i;
        return dsu[i] = getCC(dsu[i]);
    };

//    using pPP = pair<Point3, Point3>;
//    using DistancePair = pair<pPP, int64>;
    map<int64, pii> distances;
//    set<DistancePair, > distances;

    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            auto curDist = points[i].distSquared(points[j]);
            assert(distances.count(curDist) == 0);
            distances[curDist] = pair(i, j);
        }
    }

    map<pii, bool> connected;
    while (isSecondStar || k--) {
//        auto minDist = distances.begin()->first;
        auto [bestI, bestJ] = distances.begin()->second;
        distances.erase(distances.begin());
//        cout << points[bestI] << " -> " << points[bestJ] << endl;

        A iCC = getCC(bestI);
        A jCC = getCC(bestJ);
        dsu[iCC] = jCC;
        connected[pii(bestI, bestJ)] = true;

        if (isSecondStar) {
            vint64 sizes(points.size(), 0);
            for (int i = 0; i < points.size(); ++i) {
                sizes[getCC(i)]++;
            }
            if (*max_element(all(sizes)) == points.size()) {
                return points[bestI][0] * points[bestJ][0];
            }
        }
    }

    // first star
    vint64 sizes(points.size(), 0);
    for (int i = 0; i < points.size(); ++i) {
        sizes[getCC(i)]++;
    }

    sort(all(sizes), greater<>());

    int64 res = sizes[0] * sizes[1] * sizes[2];
    return res;
}

static auto star1(const std::string &filepath, int k) {
    return star(filepath, k);
}

static auto star2(const std::string &filepath) {
    return star(filepath, INF<int>);
}

int main() {
    cout << star1("example_input.txt", 10) << endl;
    cout << star1("input.txt", 1000) << endl;
    cout << star2("example_input.txt") << endl;
    cout << star2("input.txt") << endl;
    return 0;
}

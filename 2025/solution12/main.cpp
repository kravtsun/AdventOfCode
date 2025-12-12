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
using vstring = vector<string>;

static auto INF = std::numeric_limits<int>::max();

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

template<typename T>
static bool bitGet(T curSwitch, int i) {
    return curSwitch & ((T) 1 << i);
}

template<typename T>
static void bitSet(T &curSwitch, int i) {
    curSwitch = curSwitch | ((T) 1 << i);
}

void star1() {
    std::ifstream fin{EXAMPLE_INPUT_FILE};
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
        for (auto to: g[from]) {
            d[to] += d[from];
            if (!used[to]) {
                used[to] = true;
                q.push(to);
            }
        }
    }

    for (auto [v, dist]: d) {
        cout << v << " " << dist << endl;
    }

//    cout << d["out"] << endl;
}

string cycle_st, cycle_end;

map<string, char> cl;
map<string, string> p;

bool dfs(const map<string, vector<string>> &g, string v) {
    cl[v] = 1;
    if (g.count(v)) {
        for (auto to: g.at(v)) {
            if (cl[to] == 0) {
                p[to] = v;
                if (dfs(g, to)) return true;
            } else if (cl[to] == 1) {
                cycle_end = v;
                cycle_st = to;
                return true;
            }
        }
    }
    cl[v] = 2;
    return false;
}

using Shape = vector<string>;
struct State {
    int iShape;
    int iRot;
};

Shape rotateShape(const Shape &shape) {
    Shape resShape = shape;
    for (int i = 0; i < shape.size(); ++i) {
        for (int j = 0; j < shape[0].size(); ++j) {
            resShape[j][shape.size() - i - 1] = shape[i][j];
        }
    }
    return resShape;
}


int main() {
    auto lines = readLines(EXAMPLE_INPUT_FILE);
//    auto lines = readLines(INPUT_FILE);

    const int SHAPE_SIZE = 3;
    vector<Shape> shapes;
    int iLine;
    for (iLine = 0; iLine < lines.size(); ++iLine) {
        const auto &line = lines[iLine];
        istringstream is(line);
        int num;
        char c;
        is >> num >> c;
        if (c == 'x') break;
        assert(c == ':');
        vector<string> shape;
        for (int i = 0; i < SHAPE_SIZE; ++i) {
            shape.push_back(lines[iLine + i + 1]);
            assert(shape.back().size() == SHAPE_SIZE);
        }
        shapes.push_back(shape);
        iLine += SHAPE_SIZE + 1;
        assert(lines[iLine].empty());
    }

    const int NSHAPES = shapes.size();
    vector<vector<vstring>> shapeRotations(NSHAPES, vector<vstring>(4));
    for (int iShape = 0; iShape < NSHAPES; ++iShape) {
        auto curShape = shapes[iShape];
        for (int iRot = 0; iRot < 4; ++iRot) {
            shapeRotations[iShape][iRot] = curShape;
            curShape = rotateShape(curShape);
        }
    }


    // bit strings for 3 last strings
    using StringBits = int64;
    using LastStrings = tuple<StringBits, StringBits, StringBits>;
    static_assert(SHAPE_SIZE == std::tuple_size_v<LastStrings>);
    const auto stringToBits = [](const int fieldWidth, const string &s) {
        assert(fieldWidth == s.size());
        assert(s.size() < 64);
        StringBits res = 0;
        for (int i = 0; i < s.size(); ++i) {
            if (s[i] != '.') {
                bitSet(res, i);
            }
        }
        return res;
    };
    const auto bitsToStrings = [](const int fieldWidth, const StringBits &stringBits) {
        std::string res(fieldWidth, '.');
        for (int i = 0; i < fieldWidth; ++i) {
            if (bitGet(stringBits, i)) {
                res[i] = '#';
            }
        }
        return res;
    };

    // shape count into integer representation
    using ShapeCountsBits = int;
    const auto MAX_SHAPE_COUNT = 100;
    const auto shapeCountsToBits = [MAX_SHAPE_COUNT](const int nshapes, const vint &shapeCounts) {
        assert(nshapes == shapeCounts.size());
        ShapeCountsBits res = 0;
        for (const auto &sc: shapeCounts) {
            res *= MAX_SHAPE_COUNT;
            assert(sc < MAX_SHAPE_COUNT);
            res += sc;
        }
        return res;
    };
    const auto bitsToShapeCounts = [](const int nshapes, ShapeCountsBits shapeCountsBits) {
        vint shapeCounts;
        for (int i = 0; i < nshapes; ++i) {
            shapeCounts.push_back(shapeCountsBits % MAX_SHAPE_COUNT);
            shapeCountsBits /= MAX_SHAPE_COUNT;
        }
        reverse(all(shapeCounts));
        return shapeCounts;
    };

    int res = 0;
    for (int iField = 0; iLine + iField < lines.size(); ++iField) {
        const auto &line = lines[iLine + iField];
        int fieldWidth, fieldHeight;
        char c;
        istringstream is(line);
        is >> fieldWidth >> c >> fieldHeight;
        assert(c == 'x');
        is >> c;
        assert(c == ':');
        vint finalShapeCounts(NSHAPES);
        for (int i = 0; i < NSHAPES; ++i) {
            is >> finalShapeCounts[i];
        }

        auto emptyFieldString = string(fieldWidth, '.');
        auto emptyFieldStringBits = stringToBits(fieldWidth, emptyFieldString);

        using DPKey = tuple<LastStrings, ShapeCountsBits, int>;
        // LastStrings, iShape, baseFieldLineIndexDelta
        using DPTransition = tuple<LastStrings, int, int>;

        // fieldWidth -> (State, [stateDelta]).
        map<int, map<LastStrings, std::vector<DPTransition>>> dpTransitions;

        auto startState = DPKey(make_tuple(emptyFieldStringBits,
                                           emptyFieldStringBits,
                                           emptyFieldStringBits),
                                shapeCountsToBits(NSHAPES, finalShapeCounts),
                                0);

        bool curRes = false;
        set<DPKey> used;
        queue<DPKey> q;
        q.push(startState);
        used.insert(startState);
        while (!q.empty()) {
            auto curDPKey = q.front();
            const auto [lastStringsBitsTuple, shapeCountsBits, baseFieldLineIndex] = curDPKey;
            assert(baseFieldLineIndex + SHAPE_SIZE <= fieldHeight);
            q.pop();

            if (shapeCountsBits == 0) {
                curRes = true;
                break;
            }


            auto [lastStringsBits0, lastStringsBits1, lastStringsBits2] = lastStringsBitsTuple;
//            vstring lastStrings{
//                    bitsToStrings(fieldWidth, lastStringsBits0),
//                    bitsToStrings(fieldWidth, lastStringsBits1),
//                    bitsToStrings(fieldWidth, lastStringsBits2),
//            };
//
//            cout << baseFieldLineIndex << endl;
//            for (const auto& s : lastStrings) {
//                cout << s << endl;
//            }
//            cout << endl;
            auto shapeCounts = bitsToShapeCounts(NSHAPES, shapeCountsBits);
            if (dpTransitions.count(fieldWidth) && dpTransitions[fieldWidth].count(lastStringsBitsTuple)) {
                const auto &curDPTransitions = dpTransitions.at(fieldWidth).at(lastStringsBitsTuple);
                for (const auto &dpTransition : curDPTransitions) {
                    auto [nextLastStringsBitsTuple, nextIShape, nextBaseFieldLineIndexDelta] = dpTransition;
                    if (shapeCounts[nextIShape] == 0) continue;
                    if (baseFieldLineIndex + nextBaseFieldLineIndexDelta + SHAPE_SIZE >= fieldHeight) continue;
                    shapeCounts[nextIShape]--;
                    auto nextShapeCountsBits = shapeCountsToBits(NSHAPES, shapeCounts);
                    shapeCounts[nextIShape]++;
                    auto nextBaseFieldLineIndex = baseFieldLineIndex + nextBaseFieldLineIndexDelta;
                    DPKey nextState{nextLastStringsBitsTuple, nextShapeCountsBits, nextBaseFieldLineIndex};
                    if (used.count(nextState) == 0) {
                        used.insert(nextState);
                        q.push(nextState);
                    }
                }
                continue;
            }

//            vstring lastStrings{
//                    bitsToStrings(fieldWidth, lastStringsBits0),
//                    bitsToStrings(fieldWidth, lastStringsBits1),
//                    bitsToStrings(fieldWidth, lastStringsBits2),
//            };
//
//            cout << baseFieldLineIndex << endl;
//            for (const auto& s : lastStrings) {
//                cout << s << endl;
//            }
//            cout << endl;

            vector<StringBits> lastStringsBits{lastStringsBits0, lastStringsBits1, lastStringsBits2};
            for (int i = 0; i < SHAPE_SIZE; ++i) {
                lastStringsBits.push_back(emptyFieldStringBits);
//                lastStrings.push_back(emptyFieldString);
            }

            for (int iShape = 0; iShape < NSHAPES; ++iShape) {
//                if (shapeCounts[iShape] == 0) continue;
//                shapeCounts[iShape]--;
//                auto shapeCountsBitsNext = shapeCountsToBits(NSHAPES, shapeCounts);
                for (int i = 0; i <= SHAPE_SIZE && baseFieldLineIndex + i + SHAPE_SIZE <= fieldHeight; ++i) {
//                    vstring lastStringsI{lastStrings[i],
//                                         lastStrings[i + 1],
//                                         lastStrings[i + 2]};
                    vector<StringBits> lastStringsBitsI{
                            lastStringsBits[i],
                            lastStringsBits[i + 1],
                            lastStringsBits[i + 2],
                    };
                    for (int iRot = 0; iRot < 4; ++iRot) {
                        const auto &curShape = shapeRotations[iShape][iRot];
                        for (int j = 0; j + SHAPE_SIZE <= fieldWidth; ++j) {
//                            auto newLastStrings = lastStringsI;
                            auto newLastStringsBits = lastStringsBitsI;
                            bool shapeCanFit = true;
                            for (int ii = 0; ii < SHAPE_SIZE && shapeCanFit; ++ii) {
                                for (int jj = 0; jj < SHAPE_SIZE && shapeCanFit; ++jj) {
                                    if (curShape[ii][jj] == '.') continue;
//                                    if (newLastStrings[ii][j + jj] != '.') {
//                                        shapeCanFit = false;
//                                    } else {
//                                        newLastStrings[ii][j + jj] = char('A' + iShape);
//                                    }
                                    if (bitGet(newLastStringsBits[ii], j + jj)) {
                                        shapeCanFit = false;
                                    } else {
                                        bitSet(newLastStringsBits[ii], j + jj);
                                    }
                                }
                            }
                            if (shapeCanFit) {
//                                LastStrings lastStringsBitsTupleNext{
//                                        stringToBits(fieldWidth, newLastStrings[0]),
//                                        stringToBits(fieldWidth, newLastStrings[1]),
//                                        stringToBits(fieldWidth, newLastStrings[2])};
                                LastStrings lastStringsBitsTupleNext = {
                                        newLastStringsBits[0],
                                        newLastStringsBits[1],
                                        newLastStringsBits[2],
//                                        stringToBits(fieldWidth, newLastStrings[0]),
//                                        stringToBits(fieldWidth, newLastStrings[1]),
//                                        stringToBits(fieldWidth, newLastStrings[2])
                                };


//                                auto [nextLastStringsBitsTuple, nextIShape, nextBaseFieldLineIndexDelta] = dpTransition;
                                dpTransitions[fieldWidth][lastStringsBitsTuple].emplace_back(lastStringsBitsTupleNext, iShape, i);

                                if (shapeCounts[iShape] != 0) {
                                    shapeCounts[iShape]--;
                                    auto shapeCountsBitsNext = shapeCountsToBits(NSHAPES, shapeCounts);
                                    auto nextState = DPKey{lastStringsBitsTupleNext, shapeCountsBitsNext,
                                                           baseFieldLineIndex + i};
                                    if (!used.count(nextState)) {
                                        q.push(nextState);
                                        used.insert(nextState);
                                    }
                                    shapeCounts[iShape]++;
                                }
                            }
                        }
                    }
                }
//                shapeCounts[iShape]++;
            }
        }

        cout << iField << " " << (curRes ? " OK " : "FAIL") << endl;
        res += curRes;
    }

    cout << res << endl;


    return 0;
}

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
static const std::string EXAMPLE_INPUT_FILE = "example_input.txt";
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

struct State {
    vint lights;

    State(vint lights = {}) : lights(std::move(lights)) {}

    State &operator=(const State &rhs) {
        lights.resize(rhs.lights.size());
        for (int i = 0; i < lights.size(); ++i) {
            lights[i] = rhs.lights[i];
        }
        return *this;
    }
};

bool operator<(const State &lhs, const State &rhs) {
    assert(lhs.lights.size() == rhs.lights.size());
    for (int i = 0; i < lhs.lights.size(); ++i) {
        if (lhs.lights[i] != rhs.lights[i]) {
            return lhs.lights[i] < rhs.lights[i];
        }
    }
    return false;
}

bool operator==(const State &lhs, const State &rhs) {
    assert(lhs.lights.size() == rhs.lights.size());
    for (int i = 0; i < lhs.lights.size(); ++i) {
        if (lhs.lights[i] != rhs.lights[i]) {
            return false;
        }
    }
    return true;
}


bool operator!=(const State &lhs, const State &rhs) {
    assert(lhs.lights.size() == rhs.lights.size());
    return !(lhs == rhs);
}

ostream &operator<<(ostream &os, const State &s) {
    for (int i = 0; i < s.lights.size(); ++i) {
        if (i) os << ",";
        os << s.lights[i];
    }
    return os;
}

bool getBit(int curSwitch, int i) {
    return curSwitch & (1 << i);
}

State goNextState(const State &curState, int curSwitch) {
    auto newState = curState;
    for (int i = 0; i < curState.lights.size(); ++i) {
        if (getBit(curSwitch, i)) {
            newState.lights[i]++;
        }
    }
    return newState;
}

//map<int, int> switchLightsCounts;
using vbool = vector<bool>;
using vvint = vector<vint>;

static void printA(const vvint64 &a) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= m; ++j) {
            cout << a[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

static void gaussForSwitches (vvint64 &a) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;

    vint switchMapping(m);
    std::iota(all(switchMapping), 0);

    vector<int> where (m, -1);
    for (int col=0, row=0; col<m && row<n; ++col) {
        int sel = row;
        for (int i=row; i<n; ++i)
            if (abs(a[i][col]) > abs(a[sel][col]))
                sel = i;
        if (a[sel][col] == 0) {
            int newCol = -1;
            for (int j = col + 1; j < m; ++j) {
                if (abs(a[row][j]) != 0) {
                    newCol = j;
                    break;
                }
            }
            if (newCol == -1) continue;
            std::swap(switchMapping[col], switchMapping[newCol]);
            for (int i = 0; i < n; ++i) {
                std::swap(a[i][col], a[i][newCol]);
            }
            if (newCol == -1) continue;
            assert (a[sel][col] != 0);
        }
        for (int i=col; i<=m; ++i)
            swap (a[sel][i], a[row][i]);
        where[col] = row;

        for (int i=0; i<n; ++i)
            if (i != row) {
                if (abs(a[row][col]) == 1) {
                    auto c = a[i][col] / a[row][col];
                    for (int j=col; j<=m; ++j)
                        a[i][j] -= a[row][j] * c;
                } else if (a[i][col] != 0) {
                    auto tmp = std::lcm(a[i][col], a[row][col]);
                    auto c = tmp / a[i][col];
                    assert(a[i][col] * c == tmp);
//                    a[i][j] *= tmp / a[i][col];
//                    a[i][col] -= a[row][j] * (tmp / a[row][col]);
                    for (int j=0; j<=m; ++j) {
                        a[i][j] *= c;
                        a[i][j] -= a[row][j] * (tmp / a[row][col]);
                    }
                }

            }
        ++row;
    }


    auto aNew = a;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            aNew[i][switchMapping[j]] = a[i][j];
        }
    }
    a = aNew;

//    printA(a);
}

// (switchIndex, switchCount), isSuccessful
static auto getSureAnswers(vvint64 &a) {
    const auto n = a.size();
    const auto m = a[0].size() - 1;
    bool stillHasSolvedSwitches = true;
    map<int, int> res;
    while (stillHasSolvedSwitches) {
        stillHasSolvedSwitches = false;
        for (int j = 0; j < n; ++j) {
            auto countNonZero = std::count(a[j].begin(), a[j].begin() + m, 0);
            auto it = std::find_if(a[j].begin(), a[j].begin() + m, [](int64 a) { return a != 0; });
            int i = it - a[j].begin();
            if (countNonZero == m - 1) {
                stillHasSolvedSwitches = true;
                assert(it != a[j].begin() + m);
//                std::cout << "j = " << j << endl;
//                printA(a);
//                assert(a[j][m] % *it == 0);
                auto curSwitchCount = a[j][m] / *it;
                if (a[j][m] % *it != 0 || curSwitchCount < 0) {
                    // non integral solution
                    return std::make_tuple(map<int, int>{}, true),
                }
//                assert(curSwitchCount >= 0);

                for (int k = 0; k < n; ++k) {
                    a[k][m] -= curSwitchCount * a[k][i];
                    a[k][i] = 0;
                }
                res.emplace(i, curSwitchCount);
                a[j][i] = 0;
                assert(a[j][m] == 0);
//                std::cout << "j = " << j << endl;
//                printA(a);
            }
        }
    }
    return std::make_tuple(res, true);
}

void DFS(const State &finishState, const vint &switches, vbool &switchesUsed, int curRes, int &bestRes, vvint64 a) {
    assert(curRes >= 0);
    const auto nlights = finishState.lights.size();
    const auto nswitches = switches.size();

    if (curRes >= bestRes) return;
    auto maxLightsValue = *max_element(all(finishState.lights));
    if (maxLightsValue == 0) {
        bestRes = min(curRes, bestRes);
        return;
    }
    if (maxLightsValue + curRes >= bestRes) return;
    if (*min_element(all(switchesUsed))) return;

    auto minLightCnt = INF;
    auto minLightIndex = -1;
    for (int i = 0; i < nlights; ++i) {
        if (finishState.lights[i] < minLightCnt && finishState.lights[i] > 0) {
            minLightCnt = finishState.lights[i];
            minLightIndex = i;
        }
    }

    {
        State startState{vint(nlights, 0)};
        auto allSwitchesState = startState;
        for (int i = 0; i < nswitches; ++i) {
            if (switchesUsed[i]) continue;
            allSwitchesState = goNextState(allSwitchesState, switches[i]);
        }
        for (int i = 0; i < nlights; ++i) {
            if (allSwitchesState.lights[i] == 1) {
                minLightIndex = i;
                break;
            }
        }
    }

    vpii switchLightsCounts;
    for (int i = 0; i < nswitches; ++i) {
        if (switchesUsed[i] || !getBit(switches[i], minLightIndex)) continue;
        int cnt = 0;
        for (int j = 0; j < nlights; ++j) {
            if (finishState.lights[j] == 0 || !getBit(switches[i], j)) continue;
            cnt++;
        }
        switchLightsCounts.emplace_back(cnt, i);
    }
    if (switchLightsCounts.empty()) return;

    auto maxSwitchLightCount = min_element(all(switchLightsCounts));
    auto curSwitchIndex = maxSwitchLightCount->second;
    auto curSwitch = switches[curSwitchIndex];

    switchesUsed[curSwitchIndex] = true;
    int maxCount = INF;
    if (bestRes != INF) {
        maxCount = bestRes - curRes;
    }
    for (int j = 0; j < nlights; ++j) {
        if (getBit(curSwitch, j)) {
            maxCount = min(maxCount, finishState.lights[j]);
        }
    }

    auto tryNextDFS = [=, &bestRes](int curSwitchIndex, int curSwitchCnt) {
        auto curSwitch = switches[curSwitchIndex];
        int newCurRes = curRes + curSwitchCnt;
        auto newFinishState = finishState;
        for (int j = 0; j < nlights; ++j) {
            if (getBit(curSwitch, j)) {
                newFinishState.lights[j] -= curSwitchCnt;
                assert(newFinishState.lights[j] >= 0);
            }
        }

        auto newA = a;
//        printA(newA);
        for (int j = 0; j < nlights; ++j) {
            newA[j][nswitches] -= newA[j][curSwitchIndex] * curSwitchCnt;
            newA[j][curSwitchIndex] = 0;
        }

//        printA(newA);
        auto switchesUsedNew = switchesUsed;
        try {
            for (auto [switchIndex, switchCount]: getSureAnswers(newA)) {
                switchesUsedNew[switchIndex] = true;
                newCurRes += switchCount;
                for (int j = 0; j < nlights; ++j) {
                    if (getBit(switches[switchIndex], j)) {
                        newFinishState.lights[j] -= switchCount;
                        if (newFinishState.lights[j] < 0) return;
//                        assert(newFinishState.lights[j] >= 0);
                    }
                }
            }
        } catch (std::logic_error &e) {
            return;
        }
//        printA(newA);

        DFS(newFinishState, switches, switchesUsedNew, newCurRes, bestRes, newA);
    };

    if (switchLightsCounts.size() == 1) {
        tryNextDFS(curSwitchIndex, maxCount);
    } else {
        for (int i = maxCount; i >= 0; --i) {
            tryNextDFS(curSwitchIndex, i);
        }
    }

    switchesUsed[curSwitchIndex] = false;
}

int main() {
//    auto lines = readLines(EXAMPLE_INPUT_FILE);
    auto lines = readLines(INPUT_FILE);

//    std::ifstream fin{EXAMPLE_INPUT_FILE};
//    std::ifstream fin{INPUT_FILE};
//    assert(fin.is_open());

    int res = 0;

    const auto star2 = [](const std::string &line) {
        istringstream is(line);
        char c;
        is >> c;
        string indicatorLightsDiagram;
        while (c != ']') {
            is >> c;
            if (c != ']') {
                indicatorLightsDiagram.push_back(c);
            }
        }
        const auto nlights = indicatorLightsDiagram.size();

        vint switches;
        is >> c;
        while (c != '{') {
            int curSwitch = 0;
            int num;
            while (c != ')') {
                is >> num;
                is >> c;
                if (isdigit(c)) {
                    is.putback(c);
                } else {
                    assert(c == ',' || c == ')');
                }
                curSwitch = curSwitch | (1 << num);
            }
            switches.push_back(curSwitch);
            is >> c;
        }
        assert(c == '{');
        const auto nswitches = switches.size();

        vint joltage;
        while (c != '}') {
            int num;
            is >> num;
            is >> c;
            joltage.push_back(num);
        }
        assert(joltage.size() == nlights);
        auto joltageState = State{joltage};

        vvint64 a(nlights, vint64(nswitches));
        for (int j = 0; j < nlights; ++j) {
            for (int i = 0; i < nswitches; ++i) {
                a[j][i] = getBit(switches[i], j);
            }
            a[j].push_back(joltage[j]);
        }
        gaussForSwitches(a);

        int curRes = 0;
        vbool switchesUsed(nswitches, false);

        for (auto [i, curSwitchCount] : getSureAnswers(a)) {
            curRes += curSwitchCount;
            switchesUsed[i] = true;
            for (int j = 0; j < nlights; ++j) {
                if (getBit(switches[i], j)) {
                    assert(joltageState.lights[j] >= curSwitchCount);
                    joltageState.lights[j] -= curSwitchCount;
                }
            }
        }

        int bestRes = INF;
        DFS(joltageState, switches, switchesUsed, curRes, bestRes, a);
        return bestRes;
    };

    for (const auto& line : lines) {
        auto bestRes = star2(line);
        res += bestRes;
        cout << bestRes << endl;
    }

    // max switches size 13
    // max lights size 10
    // max joltage 261

    cout << res << endl;
    return 0;
}

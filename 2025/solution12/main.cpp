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

static const std::string EXAMPLE_INPUT_FILE = aoc_utils::EXAMPLE_INPUT_FILE;
static const std::string INPUT_FILE = aoc_utils::INPUT_FILE;

template<typename T = int64>
T from_string(const std::string &s) {
    std::istringstream is(s);
    T x;
    is >> x;
    return x;
}


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

template<typename T>
static bool bitGet(T curSwitch, int i) {
    return curSwitch & ((T) 1 << i);
}

template<typename T>
static void bitSet(T &curSwitch, int i) {
    curSwitch = curSwitch | ((T) 1 << i);
}


const int SHAPE_SIZE = 3;

using Shape = int;

bool shapeBitGet(Shape shape, int i, int j) {
    assert(i >= 0 && i < SHAPE_SIZE);
    assert(j >= 0 && j < SHAPE_SIZE);
    return bitGet(shape, i * SHAPE_SIZE + j);
}

void shapeBitSet(Shape &shape, int i, int j) {
    bitSet(shape, i * SHAPE_SIZE + j);
}

Shape rotateShape(const Shape &shape) {
    Shape resShape = 0;
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        for (int j = 0; j < SHAPE_SIZE; ++j) {
            if (shapeBitGet(shape, i, j)) {
                shapeBitSet(resShape, j, SHAPE_SIZE - i - 1);
            }
        }
    }
    return resShape;
}

Shape flipShape(const Shape &shape) {
    Shape resShape = 0;
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        for (int j = 0; j < SHAPE_SIZE; ++j) {
            if (shapeBitGet(shape, i, SHAPE_SIZE - 1 - j)) {
                shapeBitSet(resShape, i, j);
            }
        }
    }
    return resShape;
}

vstring shapeToStrings(const Shape &shape) {
    vstring strings(SHAPE_SIZE, string(SHAPE_SIZE, '.'));
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        for (int j = 0; j < SHAPE_SIZE; ++j) {
            if (shapeBitGet(shape, i, j)) {
                strings[i][j] = '#';
            }
        }
    }
    return strings;
}

Shape stringsToShape(const vstring &strings) {
    assert(strings.size() == SHAPE_SIZE && strings.front().size() == SHAPE_SIZE);
    Shape shape = 0;
    for (int i = 0; i < SHAPE_SIZE; ++i) {
        for (int j = 0; j < SHAPE_SIZE; ++j) {
            if (strings[i][j] != '.') {
                shapeBitSet(shape, i, j);
            }
        }
    }
    return shape;
}

int getShapeArea(const Shape &shape) {
    int res = 0;
    for (int i = 0; i < SHAPE_SIZE * SHAPE_SIZE; ++i) {
        if (bitGet(shape, i)) {
            res++;
        }
    }
    return res;
}


int main() {
//    auto lines = readLines(EXAMPLE_INPUT_FILE);
    auto lines = readLines(INPUT_FILE);

    vector<Shape> originalShapes;
    vector<int> shapeAreas;
    int iLine;
    for (iLine = 0; iLine < lines.size(); ++iLine) {
        const auto &line = lines[iLine];
        istringstream is(line);
        int num;
        char c;
        is >> num >> c;
        if (c == 'x') break;
        assert(c == ':');
        vector<string> shapeStrings;
        for (int i = 0; i < SHAPE_SIZE; ++i) {
            shapeStrings.push_back(lines[iLine + i + 1]);
            assert(shapeStrings.back().size() == SHAPE_SIZE);
        }
        originalShapes.push_back(stringsToShape(shapeStrings));
        shapeAreas.push_back(getShapeArea(originalShapes.back()));
        iLine += SHAPE_SIZE + 1;
        assert(lines[iLine].empty());
    }

    const int N_ORIGINAL_SHAPES = originalShapes.size();
    vector<set<Shape>> shapes(N_ORIGINAL_SHAPES);
//    set<Shape> allShapes;
    for (int iShape = 0; iShape < N_ORIGINAL_SHAPES; ++iShape) {
        auto curShape = originalShapes[iShape];
        for (int iRot = 0; iRot < 4; ++iRot) {
            shapes[iShape].insert(curShape);
//            allShapes.insert(curShape);
            shapes[iShape].insert(flipShape(curShape));
//            allShapes.insert(flipShape(curShape));
            curShape = rotateShape(curShape);
        }
        for_each(all(shapes[iShape]), [](const Shape &shape) {
            auto strings = shapeToStrings(shape);
            for (auto l : strings) cout << l << endl;
            cout << endl;
        });
        cout << endl;
    }
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
        vint finalShapeCounts(N_ORIGINAL_SHAPES);
        int requiredArea = 0;
        for (int i = 0; i < N_ORIGINAL_SHAPES; ++i) {
            is >> finalShapeCounts[i];
            requiredArea += finalShapeCounts[i] * shapeAreas[i];
        }
        if (requiredArea <= fieldWidth * fieldHeight) {
            res++;
        }

//        using Field = vector<vector<Shape>>;
//        Field field(fieldHeight - SHAPE_SIZE + 1, vector<Shape>(fieldWidth - SHAPE_SIZE + 1, 0));
//        auto setShape = [](const Field &field, int i, int j, Shape shape) {
//            assert((field[i][j] ^ shape) == 0);
//            for (int ii = 0; ii < ; ++ii) {
//
//            }
//        };
    }

    cout << res << endl;

    return 0;
}

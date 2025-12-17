#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cassert>
#include <queue>
#include <set>

using Point = std::pair<int, int>;

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static const int NDELTA = 4;

static Point pointAddDelta(Point p, int deltaIndex) {
    // IMPORTANT: ordered clockwise
    static const std::array<int, NDELTA> di{-1, 0, 1, 0};
    static const std::array<int, NDELTA> dj{0, 1, 0, -1};
    static_assert(di.size() == dj.size());
    return std::make_pair(p.first + di[deltaIndex], p.second + dj[deltaIndex]);
}

static auto linesToColors(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    int currentColor = 0;
    std::vector<std::vector<int>> colors(n, std::vector<int>(m, -1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (colors[i][j] == -1) {
                std::queue<Point> q;
                q.emplace(i, j);
                colors[i][j] = currentColor;
                const auto currentChar = lines[i][j];
                while (!q.empty()) {
                    auto p = q.front();
                    q.pop();
                    for (int k = 0; k < NDELTA; ++k) {
                        auto pNext = pointAddDelta(p, k);
                        if (!isGoodPoint(pNext, n, m) || colors[pNext.first][pNext.second] != -1 ||
                            lines[pNext.first][pNext.second] != currentChar) {
                            continue;
                        }
                        q.push(pNext);
                        colors[pNext.first][pNext.second] = currentColor;
                    }
                }
                currentColor++;
            }
        }
    }
    return colors;
}

static auto star1(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    const auto colors = linesToColors(lines);

    std::map<int, int> perimeters;
    std::map<int, int> areas;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < NDELTA; ++k) {
                auto pNext = pointAddDelta({i, j}, k);
                if (!isGoodPoint(pNext, n, m) || colors[pNext.first][pNext.second] != colors[i][j]) {
                    perimeters[colors[i][j]]++;
                }
            }
            areas[colors[i][j]]++;
        }
    }

    int64_t result = 0;
    assert(perimeters.size() == areas.size());
    for (auto [c, perimeter]: perimeters) {
        result += perimeter * areas.at(c);
    }
    return result;
}

static auto star2(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    const auto colors = linesToColors(lines);

    // color to small side (coordinate, index of di/dj)
    using SmallSide = std::pair<Point, int>;
    std::map<int, std::set<SmallSide>> colorSmallSides;
    std::map<int, int> colorAreas;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            const auto currentColor = colors[i][j];
            Point p{i, j};
            for (int k = 0; k < NDELTA; ++k) {
                auto pNext = pointAddDelta({i, j}, k);
                if (!isGoodPoint(pNext, n, m) || colors[pNext.first][pNext.second] != colors[i][j]) {
                    colorSmallSides[currentColor].insert(std::make_pair(p, k));
                }
            }
            colorAreas[currentColor]++;
        }
    }

    std::map<int, int> colorSidesCount;

    for (auto [currentColor, smallSides]: colorSmallSides) {
        std::set<SmallSide> used;
        for (const auto &smallSide: smallSides) {
            if (used.count(smallSide)) continue;
            auto [pStart, k] = smallSide;

            auto kPrev = (k - 1 + NDELTA) % NDELTA;
            auto kNext = (k + 1 + NDELTA) % NDELTA;

            std::queue<Point> q;
            auto addNext = [&](Point p, int kNext) {
                auto pNext = pointAddDelta(p, kNext);

                auto nextSmallSide = std::make_pair(pNext, k);
                if (!isGoodPoint(pNext, n, m) || !smallSides.count(nextSmallSide) || used.count(nextSmallSide)) return;
                q.push(pNext);
                used.insert(nextSmallSide);
            };

            q.push(pStart);
            used.insert(smallSide);
            while (!q.empty()) {
                auto p = q.front();
                q.pop();
                addNext(p, kPrev);
                addNext(p, kNext);
            }

            colorSidesCount[currentColor]++;
        }
    }

    int64_t result = 0;
    assert(colorSidesCount.size() == colorAreas.size());
    for (auto [c, sidesCount]: colorSidesCount) {
        result += sidesCount * colorAreas.at(c);
    }
    return result;
}

static void testFiles(const std::map<std::string, int64_t> &expectedOutputs, bool isStar2) {
    for (auto [filepath, expectedResult]: expectedOutputs) {
        std::ifstream fin{filepath};
        assert(fin.is_open());
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(fin, line)) {
            lines.push_back(line);
        }
        const auto actualResult = isStar2 ? star2(lines) : star1(lines);
        std::cout << filepath << ": " << actualResult << " =? " << expectedResult << std::endl;
        assert(actualResult == expectedResult);
    }
}

static void testStar1() {
    std::map<std::string, int64_t> expectedOutputs = {
            {"example_input1.txt", 140},
            {"example_input2.txt", 772},
            {"example_input3.txt", 1930},
            {aoc_utils::INPUT_FILE,          1375476},
    };

    testFiles(expectedOutputs, false);
}

static void testStar2() {
    std::map<std::string, int64_t> expectedOutputs = {
            {"example_input1.txt", 80},
            {"example_input2.txt", 436},
            {"example_input4.txt", 236},
            {"example_input5.txt", 368},
            {"example_input3.txt", 1206},
            {aoc_utils::INPUT_FILE,          821372},
    };

    testFiles(expectedOutputs, true);
}

int main() {
    testStar1();
    testStar2();
    return 0;
}

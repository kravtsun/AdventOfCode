#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <set>
#include <map>

using Point = std::pair<int, int>;

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static auto addPoint(const Point &lhs, const Point &rhs) {
    return Point{lhs.first + rhs.first, lhs.second + rhs.second};
}

static int star1(const std::vector<std::string> &lines) {
    constexpr std::string_view sample = "XMAS";
    constexpr auto sampleSize = static_cast<int>(sample.size());

    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    int result = 0;
    for (int i = 0; i < n; ++i) {
        assert(lines[i].size() == m);
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] != sample.front())
                continue;
            Point p{i, j};
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    Point pLast = addPoint(p, {(sampleSize - 1) * di, (sampleSize - 1) * dj});
                    if (!isGoodPoint(pLast, n, m)) continue;
                    bool isGood = true;
                    for (int k = 1; k < sampleSize; ++k) {
                        auto pNext = addPoint(p, {k * di, k * dj});
                        if (lines[pNext.first][pNext.second] != sample[k]) {
                            isGood = false;
                            break;
                        }
                    }
                    if (isGood) result++;
                }
            }
        }
    }
    return result;
}

int star2(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    const auto isPairGood = [](char l, char r) {
        return l == 'M' && r == 'S' ||
               l == 'S' && r == 'M';
    };
    int result = 0;
    for (int i = 1; i + 1 < n; ++i) {
        assert(lines[i].size() == m);
        for (int j = 1; j + 1 < m; ++j) {
            if (lines[i][j] != 'A') continue;
            std::array lefts{lines[i - 1][j - 1], lines[i + 1][j - 1]};
            std::array rights{lines[i - 1][j + 1], lines[i + 1][j + 1]};
            if (isPairGood(lefts[0], rights[1]) && isPairGood(lefts[1], rights[0])) {
                result++;
            }
        }
    }
    return result;
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

static auto star1(const std::string &filepath) {
    auto lines = readLines(filepath);
    return star1(lines);
}

static auto star2(const std::string &filepath) {
    auto lines = readLines(filepath);
    return star2(lines);
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 18
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl; // 2397
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 9
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl; // 1824
    return 0;
}

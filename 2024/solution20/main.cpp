#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <cassert>

using Point = std::pair<int, int>;
const Point BAD_POINT{-1, -1};

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static const std::array DELTAS = {
        Point{0, -1},
        Point{0, 1},
        Point{-1, 0},
        Point{1, 0},
};

static Point addPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static auto readLines(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line) && !line.empty()) {
        lines.push_back(line);
    }

    auto findSymbolPos = [&lines](int i, char symbol) {
        if (auto pos = lines[i].find(symbol); pos != std::string::npos) {
            return Point{i, static_cast<int>(pos)};
        } else {
            return BAD_POINT;
        }
    };

    Point startPos = BAD_POINT, finishPos = BAD_POINT;
    for (int i = 0; i < lines.size(); ++i) {
        auto symbolPos = findSymbolPos(i, 'S');
        if (symbolPos != BAD_POINT) {
            startPos = symbolPos;
        }
        symbolPos = findSymbolPos(i, 'E');
        if (symbolPos != BAD_POINT) {
            finishPos = symbolPos;
        }
    }
    assert(startPos != BAD_POINT && finishPos != BAD_POINT);
    return std::make_tuple(lines, startPos, finishPos);
}

static auto calculatePath(const std::vector<std::string> &lines, Point startPos, Point finishPos) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    // which position takes the cell in the race
    std::map<Point, size_t> pos;
    std::vector<Point> path;
    auto p = startPos;
    pos[p] = 0;
    path.push_back(p);
    for (int i = 0; p != finishPos; ++i) {
        auto nextPoint = BAD_POINT;
        for (auto delta: DELTAS) {
            auto pNext = addPoint(p, delta);
            if (isGoodPoint(pNext, n, m) && lines[pNext.first][pNext.second] == '.' && !pos.count(pNext)) {
                nextPoint = pNext;
                break;
            }
        }
        assert(nextPoint != BAD_POINT);
        pos[nextPoint] = path.size();
        path.push_back(nextPoint);
        p = nextPoint;
    }
    return path;
}

static auto cheatsSum(const std::map<int, size_t> &cheats, int saveThreshold) {
    size_t result = 0;
    for (auto [moves, cnt]: cheats) {
        if (moves >= saveThreshold) {
            result += cnt;
        }
    }
    return result;
}

static auto solve(const std::string &filepath, int saveThreshold, int distanceThreshold) {
    auto [lines, startPos, finishPos] = readLines(filepath);
    lines[startPos.first][startPos.second] = '.';
    lines[finishPos.first][finishPos.second] = '.';

    auto path = calculatePath(lines, startPos, finishPos);

    // seconds to save -> number of cheats
    std::map<int, size_t> cheats;

    for (int i = 0; i < path.size(); ++i) {
        for (int j = i + 1; j < path.size(); ++j) {
            auto distance = abs(path[i].first - path[j].first) + abs(path[i].second - path[j].second);
            if (distance < j - i && distance <= distanceThreshold) {
                cheats[j - i - distance]++;
            }
        }
    }

    return cheatsSum(cheats, saveThreshold);
}

static auto star1(const std::string &filepath) {
    return solve(filepath, filepath == aoc_utils::EXAMPLE_INPUT_FILE ? 0 : 100, 2);
}

static auto star2(const std::string &filepath) {
    return solve(filepath, filepath == aoc_utils::EXAMPLE_INPUT_FILE ? 0 : 100, 20);
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 44
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl; // 1530

    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 3081
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl; // 1033983
    return 0;
}

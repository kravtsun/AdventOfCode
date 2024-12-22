#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <set>
#include <cassert>

static const auto LEFT = '<';
static const auto RIGHT = '>';
static const auto DOWN = 'v';
static const auto UP = '^';
static const std::array dirs = {LEFT, UP, RIGHT, DOWN};
static const auto ndirs = static_cast<int>(dirs.size());
using pii = std::pair<int, int>;
const pii BAD_POINT = std::make_pair(-1, -1);

static bool isGoodCoordinate(int x, int n) {
    return 0 <= x && x < n;
}

static bool isGoodPoint(pii p, int n, int m) {
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static const std::map<char, pii> dirToDelta = {
        {LEFT,  {0,  -1}},
        {RIGHT, {0,  1}},
        {UP,    {-1, 0}},
        {DOWN,  {1,  0}},
};

static pii addPoint(const pii &lhs, const pii &rhs) {
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
        auto pos = lines[i].find(symbol);
        if (pos != std::string::npos) {
            return pii{i, static_cast<int>(pos)};
        } else {
            return BAD_POINT;
        }
    };

    pii startPos = BAD_POINT, finishPos = BAD_POINT;
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

static auto calculatePathAndPos(const std::vector<std::string> &lines, pii startPos, pii finishPos) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    // which position takes the cell in the race
    std::map<pii, int> pos;
    std::vector<pii> path;
    auto p = startPos;
    pos[p] = 0;
    path.push_back(p);
    for (int i = 0; p != finishPos; ++i) {
        auto nextPoint = BAD_POINT;
        for (int kdir = 0; kdir < ndirs; ++kdir) {
            auto delta = dirToDelta.at(dirs[kdir]);
            auto pNext = addPoint(p, delta);
            if (isGoodPoint(pNext, n, m) && lines[pNext.first][pNext.second] == '.' && !pos.count(pNext)) {
                nextPoint = pNext;
                break;
            }
        }
        assert(nextPoint != BAD_POINT);
        pos[nextPoint] = static_cast<int>(path.size());
        path.push_back(nextPoint);
        p = nextPoint;
    }
    return std::make_tuple(path, pos);
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

    auto [path, pos] = calculatePathAndPos(lines, startPos, finishPos);

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
    return solve(filepath, filepath == "example_input.txt" ? 0 : 100, 2);
}

static auto star2(const std::string &filepath) {
    return solve(filepath, filepath == "example_input.txt" ? 0 : 100, 20);
}

int main() {
    std::cout << star1("example_input.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;

    std::cout << star2("example_input.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;

    return 0;
}

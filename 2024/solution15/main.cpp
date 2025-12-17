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
using Point = std::pair<int, int>;
static const std::map<char, Point> dirToDelta = {
        {LEFT,  {0,  -1}},
        {RIGHT, {0,  1}},
        {UP,    {-1, 0}},
        {DOWN,  {1,  0}},
};

static Point addPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static Point subtractPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}

static auto readLines(std::istream &fin) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line) && !line.empty()) {
        lines.push_back(line);
    }

    const Point BAD_POINT = std::make_pair(-1, -1);
    Point startPos = BAD_POINT;
    for (int i = 0; i < lines.size(); ++i) {
        if (auto pos = lines[i].find('@'); pos != std::string::npos) {
            startPos.first = i;
            startPos.second = static_cast<int>(pos);
            break;
        }
    }
    assert(startPos != BAD_POINT);
    return std::make_tuple(lines, startPos);
}

static auto measureDistances(const std::vector<std::string> &lines, char symbol) {
    const auto n = lines.size();
    const auto m = lines[0].size();
    size_t result = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (lines[i][j] == symbol) {
                result += i * 100 + j;
            }
        }
    }
    return result;
}

static auto star1(const std::string &filepath) {
    std::ifstream fin{filepath};
    auto [lines, startPos] = readLines(fin);
    lines[startPos.first][startPos.second] = '.';

    auto p = startPos;
    std::string moves;
    while (std::getline(fin, moves)) {
        assert(!moves.empty());
        for (auto dirChar: moves) {
            auto delta = dirToDelta.at(dirChar);
            Point pNext = addPoint(p, delta);
            if (lines[pNext.first][pNext.second] == '#') continue;
            Point pBlock = pNext;
            while (lines[pBlock.first][pBlock.second] == 'O') {
                pBlock = addPoint(pBlock, delta);
            }
            if (lines[pBlock.first][pBlock.second] == '.') {
                for (auto pp = pBlock; pp != pNext; pp = subtractPoint(pp, delta)) {
                    lines[pp.first][pp.second] = 'O';
                }
                lines[pNext.first][pNext.second] = '.';
                p = pNext;
            }
        }
    }

    return measureDistances(lines, 'O');
}

// Modified field specially for star2
static auto getNewLines(const std::vector<std::string> &lines, Point startPos) {
    const auto n = lines.size();
    const auto m = lines[0].size();
    std::vector<std::string> newLines(n, std::string(2 * m, '.'));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (lines[i][j] == 'O') {
                newLines[i][2 * j + 0] = '[';
                newLines[i][2 * j + 1] = ']';
            } else {
                newLines[i][2 * j + 0] = lines[i][j];
                newLines[i][2 * j + 1] = lines[i][j];
            }
        }
    }
    startPos.second *= 2;
    return std::make_tuple(newLines, startPos);
}

static auto star2(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    auto [lines, startPos] = readLines(fin);
    lines[startPos.first][startPos.second] = '.';
    auto [newLines, newStartPos] = getNewLines(lines, startPos);
    lines = newLines;
    startPos = newStartPos;

    auto isObstacle = [&](Point pNext) {
        return lines[pNext.first][pNext.second] == '#';
    };

    auto p = startPos;
    std::string moves;
    while (std::getline(fin, moves)) {
        assert(!moves.empty());
        for (auto dirChar: moves) {
            auto delta = dirToDelta.at(dirChar);
            Point pNext = addPoint(p, delta);
            if (isObstacle(pNext)) continue;

            if (lines[pNext.first][pNext.second] == '.') {
                p = pNext;
                continue;
            }
            if (dirChar == LEFT || dirChar == RIGHT) {
                auto pBlock = pNext;
                while (lines[pBlock.first][pBlock.second] == ']' || lines[pBlock.first][pBlock.second] == '[') {
                    pBlock = addPoint(pBlock, delta);
                }
                if (isObstacle(pBlock)) continue;
                while (pBlock != pNext) {
                    auto pPrevBlock = subtractPoint(pBlock, delta);
                    lines[pBlock.first][pBlock.second] = lines[pPrevBlock.first][pPrevBlock.second];
                    pBlock = pPrevBlock;
                }
                lines[pBlock.first][pBlock.second] = '.';
            } else {
                assert(dirChar == UP || dirChar == DOWN);
                auto pStartBlock = pNext;
                if (lines[pStartBlock.first][pStartBlock.second] == ']') {
                    pStartBlock.second--;
                }

                std::set<Point> used; // points with '['
                std::queue<Point> q;
                q.push(pStartBlock);
                used.insert(pStartBlock);
                while (!q.empty()) {
                    Point pBlock = q.front();
                    q.pop();
                    assert(lines[pBlock.first][pBlock.second] == '[');
                    auto pNextBlock = addPoint(pBlock, delta);
                    auto pNextBlockRight = addPoint(pNextBlock, dirToDelta.at(RIGHT));
                    auto pNextBlockLeft = addPoint(pNextBlock, dirToDelta.at(LEFT));
                    if (isObstacle(pNextBlock) || isObstacle(pNextBlockRight)) continue;
                    if (lines[pNextBlock.first][pNextBlock.second] == '[') {
                        assert(lines[pNextBlock.first][pNextBlock.second + 1] == ']');
                        q.push(pNextBlock);
                        used.insert(pNextBlock);
                    }

                    if (lines[pNextBlock.first][pNextBlock.second] == ']' && used.count(pNextBlockLeft) == 0) {
                        assert(lines[pNextBlockLeft.first][pNextBlockLeft.second] == '[');
                        q.push(pNextBlockLeft);
                        used.insert(pNextBlockLeft);
                    }

                    if (lines[pNextBlockRight.first][pNextBlockRight.second] == '[' &&
                        used.count(pNextBlockRight) == 0) {
                        assert(lines[pNextBlockRight.first][pNextBlockRight.second + 1] == ']');
                        q.push(pNextBlockRight);
                        used.insert(pNextBlockRight);
                    }
                }

                bool isGoodToPush = true;
                for (auto pBlock: used) {
                    auto pNextBlock = addPoint(pBlock, delta);
                    auto pNextBlockRight = addPoint(pNextBlock, dirToDelta.at(RIGHT));
                    if (isObstacle(pNextBlock) || isObstacle(pNextBlockRight)) {
                        isGoodToPush = false;
                        break;
                    }
                }
                if (!isGoodToPush) continue;

                // Passing through all the used blocks in the order by which no clashing between them happens.
                std::vector<Point> usedBlocks(used.begin(), used.end());
                std::sort(usedBlocks.begin(), usedBlocks.end(), [&dirChar](const Point &lhs, const Point &rhs) {
                    if (dirChar == UP) {
                        // higher blocks go first - they have the space already.
                        return lhs.first < rhs.first;
                    } else {
                        return lhs.first > rhs.first;
                    }
                });

                for (auto pBlock: usedBlocks) {
                    auto pNextBlock = addPoint(pBlock, delta);
                    lines[pNextBlock.first][pNextBlock.second] = lines[pBlock.first][pBlock.second];
                    lines[pNextBlock.first][pNextBlock.second + 1] = lines[pBlock.first][pBlock.second + 1];
                    lines[pBlock.first][pBlock.second] = '.';
                    lines[pBlock.first][pBlock.second + 1] = '.';
                }
            }
            p = pNext;
        }
    }

    return measureDistances(lines, '[');
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl; // 10092
    std::cout << star1("example_input2.txt") << std::endl; // 2028
    std::cout << star1("input.txt") << std::endl; // 1476771

    std::cout << star2("example_input3.txt") << std::endl; // 618
    std::cout << star2("example_input1.txt") << std::endl; // 9021
    std::cout << star2("input.txt") << std::endl; // 1468005
    return 0;
}

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
static const std::map<char, pii> dirToDelta = {
        {LEFT,  {0,  -1}},
        {RIGHT, {0,  1}},
        {UP,    {-1, 0}},
        {DOWN,  {1,  0}},
};

static pii addPoint(const pii &lhs, const pii &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static pii subtractPoint(const pii &lhs, const pii &rhs) {
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}

static auto readLines(std::istream &fin) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line) && !line.empty()) {
        lines.push_back(line);
    }

    const pii BAD_POINT = std::make_pair(-1, -1);
    pii startPos = BAD_POINT;
    for (int i = 0; i < lines.size(); ++i) {
        auto pos = lines[i].find('@');
        if (pos != std::string::npos) {
            startPos.first = i;
            startPos.second = static_cast<int>(pos);
            break;
        }
    }
    assert(startPos != BAD_POINT);
    return std::make_tuple(lines, startPos);
}

static int star1(const std::string &filepath) {
    std::ifstream fin{filepath};
    auto [lines, startPos] = readLines(fin);
    lines[startPos.first][startPos.second] = '.';
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    auto p = startPos;
    std::string moves;
    while (std::getline(fin, moves)) {
        assert(!moves.empty());
        for (auto dirChar: moves) {
            auto delta = dirToDelta.at(dirChar);
            pii pNext = addPoint(p, delta);
            if (lines[pNext.first][pNext.second] == '#') continue;
            pii pBlock = pNext;
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

    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == 'O') {
                result += i * 100 + j;
            }
        }
    }
    return result;
}

static auto measureDistances(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '[') {
                result += i * 100 + j;
            }
        }
    }
    return result;
}

static int star2(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    auto [lines, startPos] = readLines(fin);
    lines[startPos.first][startPos.second] = '.';
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    {
        std::vector<std::string> newLines(n, std::string(2 * m, '.'));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
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
        lines = newLines;
    }

    auto isHurdle = [&](pii pNext) {
        return lines[pNext.first][pNext.second] == '#';
    };

    auto p = startPos;
    std::string moves;
    while (std::getline(fin, moves)) {
        assert(!moves.empty());
        for (auto dirChar: moves) {
            auto delta = dirToDelta.at(dirChar);
            pii pNext = addPoint(p, delta);
            if (isHurdle(pNext)) continue;

            if (lines[pNext.first][pNext.second] == '.') {
                p = pNext;
                continue;
            }
            if (dirChar == LEFT || dirChar == RIGHT) {
                auto pBlock = pNext;
                while (lines[pBlock.first][pBlock.second] == ']' || lines[pBlock.first][pBlock.second] == '[') {
                    pBlock = addPoint(pBlock, delta);
                }
                if (isHurdle(pBlock)) continue;
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

                std::set<pii> used; // points with '['
                std::queue<pii> q;
                q.push(pStartBlock);
                used.insert(pStartBlock);
                while (!q.empty()) {
                    pii pBlock = q.front();
                    q.pop();
                    assert(lines[pBlock.first][pBlock.second] == '[');
                    auto pNextBlock = addPoint(pBlock, delta);
                    auto pNextBlockRight = addPoint(pNextBlock, dirToDelta.at(RIGHT));
                    auto pNextBlockLeft = addPoint(pNextBlock, dirToDelta.at(LEFT));
                    if (isHurdle(pNextBlock) || isHurdle(pNextBlockRight)) continue;
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

                    if (lines[pNextBlockRight.first][pNextBlockRight.second] == '[' && used.count(pNextBlockRight) == 0) {
                        assert(lines[pNextBlockRight.first][pNextBlockRight.second + 1] == ']');
                        q.push(pNextBlockRight);
                        used.insert(pNextBlockRight);
                    }
                }

                bool isGoodToPush = true;
                for (auto pBlock : used) {
                    auto pNextBlock = addPoint(pBlock, delta);
                    auto pNextBlockRight = addPoint(pNextBlock, dirToDelta.at(RIGHT));
                    if (isHurdle(pNextBlock) || isHurdle(pNextBlockRight)) {
                        isGoodToPush = false;
                        break;
                    }
                }
                if (!isGoodToPush) continue;
                auto newLines = lines;
                for (auto pBlock : used) {
                    newLines[pBlock.first][pBlock.second] = '.';
                    newLines[pBlock.first][pBlock.second + 1] = '.';
                }
                for (auto pBlock : used) {
                    auto pNextBlock = addPoint(pBlock, delta);
                    newLines[pNextBlock.first][pNextBlock.second] = lines[pBlock.first][pBlock.second];
                    newLines[pNextBlock.first][pNextBlock.second + 1] = lines[pBlock.first][pBlock.second + 1];
                }
                lines = newLines;
            }
            p = pNext;
        }
    }

    return measureDistances(lines);
}

int main() {
//    std::cout << star1("example_input1.txt") << std::endl;
//    std::cout << star1("example_input2.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;

//    std::cout << star2("example_input3.txt") << std::endl;
//    std::cout << star2("example_input1.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;

    return 0;
}

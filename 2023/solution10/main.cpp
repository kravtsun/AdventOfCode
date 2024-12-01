#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <set>
#include <queue>
#include <fstream>

using vint = std::vector<int>;

const std::string NORTH = "north";
const std::string SOUTH = "south";
const std::string WEST = "west";
const std::string EAST = "east";


struct Delta {
    explicit Delta(int dy, int dx) : dy(dy), dx(dx) {}

    friend bool operator<(const Delta &lhs, const Delta &rhs) {
        if (lhs.dy != rhs.dy) {
            return lhs.dy < rhs.dy;
        }

        return lhs.dx < rhs.dx;
    }

    bool isValid() const {
        return dy != 0 || dx != 0;
    }

    int dy;
    int dx;
};

static const Delta INVALID_DELTA = Delta{0, 0};

static const std::map<char, std::pair<std::string, std::string>> pipeToDirections = {
        {'|', {NORTH, SOUTH}},
        {'-', {EAST,  WEST}},
        {'L', {NORTH, EAST}},
        {'J', {NORTH, WEST}},
        {'7', {SOUTH, WEST}},
        {'F', {SOUTH, EAST}},
};

// here we put delta **to** that direction,
// in order to get delta **from** that direction - negate
static const std::map<std::string, Delta> directionToDelta = {
        {NORTH, Delta{-1, 0}},
        {SOUTH, Delta{1, 0}},
        {WEST,  Delta{0, -1}},
        {EAST,  Delta{0, 1}},
};

static const std::map<Delta, std::string> deltaToDirection = {
        {Delta{-1, 0}, NORTH},
        {Delta{1, 0},  SOUTH},
        {Delta{0, -1}, WEST},
        {Delta{0, 1},  EAST},
};

std::string inverseDirection(const std::string &direction) {
    if (direction == NORTH) {
        return SOUTH;
    } else if (direction == SOUTH) {
        return NORTH;
    } else if (direction == WEST) {
        return EAST;
    } else if (direction == EAST) {
        return WEST;
    }
    assert(false);
}

static Delta getDelta(char pipe, const Delta &previousDelta) {
    if (pipe == '.' || pipe == 'S') {
        return INVALID_DELTA;
    }
    const auto [dir1, dir2] = pipeToDirections.at(pipe);
    const std::string &previousDirection = deltaToDirection.at(previousDelta);
    const auto fromDirection = inverseDirection(previousDirection);
    if (dir1 == fromDirection) {
        return directionToDelta.at(dir2);
    } else if (dir2 == fromDirection) {
        return directionToDelta.at(dir1);
    }
    return INVALID_DELTA;
}

using pii = std::pair<int, int>;

pii applyDelta(pii p, const Delta &delta) {
    p.first += delta.dy;
    p.second += delta.dx;
    return p;
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::vector<std::string> field;
    std::string line;

    pii startPoint{-1, -1};
    for (int i = 0; std::getline(std::cin, line); ++i) {
        const auto pos = line.find('S');
        if (pos != std::string::npos) {
            startPoint = pii(i, pos);
            startPoint = applyDelta(startPoint, Delta{1, 1}); // compensate for borders.
        }

        line = "." + line + ".";

        field.push_back(line);
    }
    assert(startPoint != pii(-1, -1));

    line.clear();
    std::generate_n(std::back_inserter(line), field.back().size(), []() {
        return '.';
    });
    field.insert(field.begin(), line);
    field.insert(field.end(), line);

    //.....
    //.S-7.
    //.|.|.
    //.L-J.
    //.....
    int maxLoopLength = -1;
    std::vector<pii> maxLoopStack;
    std::vector<Delta> maxDeltaStack;
    for (auto [direction, delta]: directionToDelta) {
        std::vector<pii> pointStack;
        std::vector<Delta> deltaStack;
        pointStack.push_back(startPoint);
        auto p = applyDelta(startPoint, delta);
        deltaStack.push_back(delta);
        pointStack.push_back(p);

        do {
            delta = getDelta(field[p.first][p.second], delta);
            if (!delta.isValid()) {
                break;
            }
            p = applyDelta(p, delta);
            deltaStack.push_back(delta);
            pointStack.push_back(p);
        } while (p != startPoint);

        if (pointStack.back() == startPoint && (int) pointStack.size() - 1 > maxLoopLength) {
            pointStack.pop_back();
            maxLoopLength = (int) pointStack.size();
            maxLoopStack = pointStack;
            maxDeltaStack = deltaStack;
        }
    }

    std::set<pii> loopPoints{maxLoopStack.begin(), maxLoopStack.end()};

    const int height = field.size();
    const int width = field[0].size();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (loopPoints.count(pii(i, j)) == 0) {
                field[i][j] = '.';
            }
        }
    }

    auto getCurrentDirs = [&maxDeltaStack](const int index) -> std::pair<std::string, std::string>
    {
        const auto sDelta = maxDeltaStack[index];
        // direction outgoing from 'S'
        const auto outgoingDir = deltaToDirection.at(sDelta);

        const auto prevDelta = index == 0 ? maxDeltaStack.back() : maxDeltaStack[index - 1];
        const auto incomingDir = inverseDirection(deltaToDirection.at(prevDelta));

        return std::make_pair(incomingDir, outgoingDir);
    };

    char startPipe = 0;
    {// Need to remove 'S' letter.
        const auto [incomingDir, outgoingDir] = getCurrentDirs(0);
        for (auto [pipe, dirs]: pipeToDirections) {
            if (dirs == std::make_pair(incomingDir, outgoingDir) ||
                dirs == std::make_pair(outgoingDir, incomingDir)) {
                startPipe = pipe;
            }
        }
    }

    assert(startPipe != 0);

    field[maxLoopStack.front().first][maxLoopStack.front().second] = startPipe;

    const auto paint = [&field, &height, &width](pii startPoint, char color) {
        // Paint points (BFS)
        std::queue<pii> q;
        q.emplace(startPoint.first, startPoint.second);

        while (!q.empty()) {
            auto v = q.front();
            q.pop();

            if (field[v.first][v.second] != '.') continue;
            field[v.first][v.second] = color;
            const int DX[] = {0, 0, -1, 1};
            const int DY[] = {-1, 1, 0, 0};
            for (int k = 0; k < 4; ++k) {
                const auto dx = DX[k];
                const auto dy = DY[k];
                const auto newY = v.first + dy;
                if (newY < 0 || newY >= height) {
                    continue;
                }
                const auto newX = v.second + dx;
                if (newX < 0 || newX >= width) {
                    continue;
                }
                q.emplace(newY, newX);
            }
        }
    };

    paint(pii(0, 0), 'O');

    // find positioning for outside color
    //         {'|', {NORTH, SOUTH}},
    //        {'-', {EAST,  WEST}},
    //        {'L', {NORTH, EAST}},
    //        {'J', {NORTH, WEST}},
    //        {'7', {SOUTH, WEST}},
    //        {'F', {SOUTH, EAST}},
    assert(maxLoopStack.size() == maxDeltaStack.size());
    assert(maxLoopStack.size() == maxLoopLength);

    const auto isLoopPoint = [&maxLoopStack](pii p) -> bool {
        return std::find(maxLoopStack.begin(), maxLoopStack.end(), p) != maxLoopStack.end();
    };

    auto getIsBackgroundFirst = [&]() -> bool {
        bool initialBackgroundIsFirst = true;
        bool backgroundIsFirst = initialBackgroundIsFirst;
        for (int i = 0; i < maxLoopStack.size(); ++i) {
            auto p = maxLoopStack[i];
            auto pipe = field[p.first][p.second];
            if (pipe == '|' || pipe == '-') {
                pii firstPoint, secondPoint;
                if (pipe == '|') {
                    firstPoint = applyDelta(p, directionToDelta.at(WEST));
                    secondPoint = applyDelta(p, directionToDelta.at(EAST));
                } else {
                    firstPoint = applyDelta(p, directionToDelta.at(NORTH));
                    secondPoint = applyDelta(p, directionToDelta.at(SOUTH));
                }

                auto firstPipe = field[firstPoint.first][firstPoint.second];
                auto secondPipe = field[secondPoint.first][secondPoint.second];
                if (firstPipe == 'O') {
                    if (backgroundIsFirst) {
                        break; // initialBackgroundIsFirst was correct.
                    } else {
                        initialBackgroundIsFirst = !initialBackgroundIsFirst;
                        break;
                    }
                } else if (secondPipe == 'O') {
                    if (backgroundIsFirst) {
                        initialBackgroundIsFirst = !initialBackgroundIsFirst;
                    } else {
                        break; // initialBackgroundIsFirst was correct.
                    }
                }
            } else if (pipe == 'L') {
                backgroundIsFirst = !backgroundIsFirst;
            } else if (pipe == 'J') {
                // firstness stays the same.
            } else if (pipe == '7') {
                backgroundIsFirst = !backgroundIsFirst;
            } else if (pipe == 'F') {
                // firstness stays the same.
            } else {
                assert(false);
            }
        }

        return initialBackgroundIsFirst;
    };

    const auto backgroundIsFirst = getIsBackgroundFirst();

    char firstColor, secondColor;
    if (backgroundIsFirst) {
        firstColor = 'O', secondColor = 'I';
    } else {
        firstColor = 'I', secondColor = 'O';
    }

    for (int i = 0; i < maxLoopStack.size(); ++i) {
        auto p = maxLoopStack[i];
        auto pipe = field[p.first][p.second];
        if (pipe == '|' || pipe == '-') {
            pii firstPoint, secondPoint;
            if (pipe == '|') {
                firstPoint = applyDelta(p, directionToDelta.at(WEST));
                secondPoint = applyDelta(p, directionToDelta.at(EAST));
            } else {
                firstPoint = applyDelta(p, directionToDelta.at(NORTH));
                secondPoint = applyDelta(p, directionToDelta.at(SOUTH));
            }

            if (!isLoopPoint(firstPoint)) {
                paint(firstPoint, firstColor);
            }
            if (!isLoopPoint(secondPoint)) {
                paint(secondPoint, secondColor);
            }

        } else if (pipe == 'L' || pipe == '7') {
            std::vector<Delta> otherDeltas;
            const auto thisDirs = pipeToDirections.at(pipe);
            for (auto [dir, delta]: directionToDelta) {
                if (dir != thisDirs.first && dir != thisDirs.second) {
                    otherDeltas.push_back(delta);
                }
            }

            const auto paintOthers = [&otherDeltas, &paint, &p](const char color)
            {
                for (auto delta : otherDeltas) {
                    auto otherP = applyDelta(p, delta);
                    paint(otherP, color);
                }
            };

            const auto [incomingDir, outgoingDir] = getCurrentDirs(i);

            const auto previousDirs = getCurrentDirs((i - 1 + maxLoopStack.size()) % maxLoopStack.size());
            const auto nextDirs = getCurrentDirs((i + 1 + maxLoopStack.size()) % maxLoopStack.size());

            if (pipe == 'L') {
                if (previousDirs.second == SOUTH) {
                    paintOthers(firstColor);
                } else if (previousDirs.second == WEST) {
                    paintOthers(secondColor);
                } else {
                    assert(false);
                }
            } else {
                if (previousDirs.second == EAST) {
                    paintOthers(firstColor);
                } else if (previousDirs.second == NORTH) {
                    paintOthers(secondColor);
                } else {
                    assert(false);
                }
            }

            std::swap(firstColor, secondColor);
        } else if (pipe == 'J' || pipe == 'F') {
            // firstness stays the same.
            std::vector<Delta> otherDeltas;
            for (auto [dir, delta]: directionToDelta) {
                if (dir != pipeToDirections.at(pipe).first && dir != pipeToDirections.at(pipe).second) {
                    otherDeltas.push_back(delta);
                }
            }

            for (auto delta: otherDeltas) {
                const auto otherPoint = applyDelta(p, delta);
                if (!isLoopPoint(otherPoint)) {
                    paint(otherPoint, pipe == 'J' ? secondColor : firstColor);
                }
            }

        } else {
            assert(false);
        }
    }

    int result = 0;
    for (auto s: field) {
        result += std::count(s.begin(), s.end(), 'I');
        assert(std::count(s.begin(), s.end(), '.') == 0);
        std::cout << s << std::endl;
    }

    std::cout << result << std::endl;

    return 0;
}

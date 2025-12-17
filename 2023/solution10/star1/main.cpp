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

static const std::map<char, std::pair<std::string, std::string>> pipeToConnections = {
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

std::string inverseDirection(const std::string& direction) {
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
    const auto [dir1, dir2] = pipeToConnections.at(pipe);
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
    FILE *f = freopen(WORKDIR "input.txt", "r", stdin);

    std::vector<std::string> field;
    std::string line;

    pii startPoint{-1, -1};
    for (int i = 0; std::getline(std::cin, line); ++i) {

        const auto pos = line.find('S');
        if (pos != std::string::npos) {
            startPoint = std::make_pair(i, pos);
            startPoint = applyDelta(startPoint, Delta{1, 1}); // compensate for borders.
        }

        line = "." + line + ".";

        field.push_back(line);
    }
    assert(startPoint != std::make_pair(-1, -1));

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
    for (auto [direction, delta]: directionToDelta) {
        std::vector<pii> pointStack;
        pointStack.push_back(startPoint);
        auto p = applyDelta(startPoint, delta);
        pointStack.push_back(p);

        do {
            delta = getDelta(field[p.first][p.second], delta);
            if (!delta.isValid()) {
                break;
            }
            p = applyDelta(p, delta);
            pointStack.push_back(p);
        } while (p != startPoint);

        if (pointStack.back() == startPoint) {
            maxLoopLength = std::max(maxLoopLength, (int) pointStack.size() - 1);
        }
    }

    std::cout << maxLoopLength / 2 << std::endl;

    fclose(f);
    return 0;
}

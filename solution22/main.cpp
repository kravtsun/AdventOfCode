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
#include <memory>

using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
using vpii = std::vector<pii>;
using spii = std::set<pii>;
using vs = std::vector<std::string>;
using vbool = std::vector<bool>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

struct Brick {

    explicit Brick(const vpii &pp, const int index)
            : p(pp)
            , index(index)
    {
        for (int i = 0; i < 3; ++i) {
            if (p[i].first > p[i].second) {
                std::swap(p[i].first, p[i].second);
            }
        }

        for (int x = p[0].first; x <= p[0].second; ++x) {
            for (int y = p[1].first; y <= p[1].second; ++y) {
                busyPoints.emplace(x, y);
            }
        }
    }

    bool intersects(const Brick& rhs) const
    {
        if (intersectionMap.count(rhs.index)) {
            intersectionMap.at(rhs.index);
        }

        bool result = false;
        for (int x = rhs.p[0].first; x <= rhs.p[0].second && !result; ++x) {
            for (int y = rhs.p[1].first; y <= rhs.p[1].second && !result; ++y) {
                if (busyPoints.count(pii(x, y))) {
                    result = true;
                }
            }
        }

        return intersectionMap[rhs.index] = result;
    }

    vpii p{3};
    bool isFreezed = false;
    int index;

    mutable std::map<int, bool> intersectionMap;
    std::set<pii> busyPoints;
};

static bool compByZ(const Brick &lhs, const Brick &rhs) {
    return lhs.p[2].second < rhs.p[2].first;
}

static bool canFallFurther(const std::vector<Brick>& bricks, int i) {
    const Brick& brick = bricks[i];
    if (brick.p[2].first == 1) return false;
    assert(brick.p[2].second != 1);
    for (int j = 0; j < bricks.size(); ++j) {
        const Brick &otherBrick = bricks[j];
        if (i == j || brick.p[2].first != otherBrick.p[2].second + 1) continue;

        if (brick.intersects(otherBrick)) {
            return false;
        }
    }
    return true;
};

static bool tryMoveBricks(std::vector<Brick>& bricks) {
//    std::sort(bricks.begin(), bricks.end(), compByZ);
    vint moved;
    for (int i = 0; i < bricks.size(); ++i) {
        if (canFallFurther(bricks, i)) {
            moved.push_back(i);
//            assert(!bricks[i].isFreezed);
            bricks[i].p[2].first--;
            bricks[i].p[2].second--;
        } else {
            bricks[i].isFreezed = true;
        }
    }

    return !moved.empty();
};

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
//    std::ifstream fin{WORKDIR "input_sample.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::vector<Brick> bricks;
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        std::istringstream lineStream{line};
        vpii p(3);
        char c;
        for (int i = 0; i < 3; ++i) {
            lineStream >> p[i].first;
            lineStream >> c;
            assert(c == '~' || c == ',');
        }

        for (int i = 0; i < 3; ++i) {
            lineStream >> p[i].second >> c;
            assert(c == ',');
        }

        const int currentIndex = bricks.size();
        bricks.emplace_back(p, currentIndex);
        assert(bricks.back().p[2].first > 0 && bricks.back().p[2].second > 0);
    }

    while (true) {
        if (!tryMoveBricks(bricks)) {
            break;
        }
    }

    vint canBeRemoved;
    for (int i = 0; i < bricks.size(); ++i) {
        auto newBricks = bricks;
        newBricks.erase(newBricks.begin() + i);
        if (!tryMoveBricks(newBricks)) {
            canBeRemoved.push_back(i);
        }
    }

    std::cout << canBeRemoved.size() << std::endl;

    return 0;
}
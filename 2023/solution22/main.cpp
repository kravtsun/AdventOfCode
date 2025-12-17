#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <memory>
#include <numeric>
#include <queue>
#include <set>

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
            return intersectionMap.at(rhs.index);
        }

        bool result = false;
        for (int x = rhs.p[0].first; x <= rhs.p[0].second && !result; ++x) {
            for (int y = rhs.p[1].first; y <= rhs.p[1].second && !result; ++y) {
                if (busyPoints.count(pii(x, y))) {
                    result = true;
                }
            }
        }

        rhs.intersectionMap[this->index] = result;
        intersectionMap[rhs.index] = result;
        return result;
    }

    friend bool operator<(const Brick &lhs, const Brick &rhs)
    {
        if (lhs.p[2].second != rhs.p[2].second) {
            return lhs.p[2].second < rhs.p[2].second;
        }
        return false;
//        return lhs.p[2].first < rhs.p[2].first;
    }

    vpii p{3};
    int index;

    mutable std::map<int, bool> intersectionMap;
    std::set<pii> busyPoints;
};

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
    vint moved;
    for (int i = 0; i < bricks.size(); ++i) {
        if (canFallFurther(bricks, i)) {
            moved.push_back(i);
            bricks[i].p[2].first--;
            bricks[i].p[2].second--;
        }
    }

    return !moved.empty();
};

static void star1(const std::vector<Brick>& bricks)
{
    std::set<int> cannotBeRemoved;

    std::set<int> zz;
    for (const auto& b : bricks) {
        zz.insert(b.p[2].first);
        zz.insert(b.p[2].second);
    }

    for (auto z : zz) {
        std::vector<Brick> currentLevelBricks, nextLevelBricks;
        std::copy_if(bricks.begin(), bricks.end(), std::back_inserter(currentLevelBricks), [z](const Brick& brick) {
            return brick.p[2].second == z;
        });
        std::copy_if(bricks.begin(), bricks.end(), std::back_inserter(nextLevelBricks), [z](const Brick& brick) {
            return brick.p[2].first == z + 1;
        });

        // nextLevelIndex -> thisLevel (bricksIndex)
        std::map<int, vint> intersections;
        for (int i = 0; i < currentLevelBricks.size(); ++i) {
            for (int j = 0; j < nextLevelBricks.size(); ++j) {
                if (currentLevelBricks[i].intersects(nextLevelBricks[j])) {
                    intersections[j].push_back(currentLevelBricks[i].index);
                }
            }
        }

        for (const auto& [nextLevelIndex, thisLevels] : intersections) {
            assert(!thisLevels.empty());
            if (thisLevels.size() == 1) {
                cannotBeRemoved.insert(bricks[thisLevels.front()].index);
            }
        }
    }

    std::cout << bricks.size() - cannotBeRemoved.size() << std::endl;
}

static bool isSupported(const Brick& lowerBrick, const Brick& upperBrick) {
    return lowerBrick.p[2].second + 1 == upperBrick.p[2].first && lowerBrick.intersects(upperBrick);
};

static vvint getParents(const std::vector<Brick>& bricks)
{
    vvint parents(bricks.size());
    for (const auto& lowerBrick : bricks) {
        for (const auto& upperBrick : bricks) {
            if (lowerBrick.index == upperBrick.index) continue;
            if (isSupported(lowerBrick, upperBrick)) {
                parents[upperBrick.index].push_back(lowerBrick.index);
            }
        }
    }
    return parents;
}

static vvint getGraph(const vvint& parents)
{
    vvint g(parents.size());
    for (int upperBrickIndex = 0; upperBrickIndex < parents.size(); ++upperBrickIndex) {
//        if (parents[upperBrickIndex].size() == 1) {
//            g[parents[upperBrickIndex].front()].push_back(upperBrickIndex);
//        }
        for (auto lowerBrickIndex : parents[upperBrickIndex]) {
            g[lowerBrickIndex].push_back(upperBrickIndex);
        }
    }
    return g;
}

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

//    std::sort(bricks.begin(), bricks.end());
//    for (int i = 0; i < bricks.size(); ++i) {
//        bricks[i].index = i;
//        bricks[i].intersectionMap.clear();
//    }

    const auto parents = getParents(bricks);
    const auto g = getGraph(parents);

    const auto BFS = [&bricks, &g, &parents](int s) {
//        // queue of dropped bricks, prioritized by last z.
//        std::set<Brick> q;
//        q.insert(bricks[s]);
//
//        std::set<int> dropped;
//        dropped.insert(s);
//
//        std::set<int> alreadyProcessed;
//        alreadyProcessed.insert(s);
//        while (!q.empty()) {
//            const auto brick = *q.begin();
//            q.erase(q.begin());
//
//            for (auto to : g[brick.index]) {
//                bool anyParentsLeft = false;
//                for (auto parent : parents[to]) {
//                    if (!dropped.count(parent)) {
//                        anyParentsLeft = true;
//                        break;
//                    }
//                }
//                if (!anyParentsLeft) {
//                    assert(alreadyProcessed.count(to) == 0);
//                    alreadyProcessed.insert(to);
//                    dropped.insert(to);
//                    q.insert(bricks[to]);
//                }
//            }
//        }
//        return dropped.size() - 1;
        std::set<int> dropped;
        dropped.insert(s);

        bool newDroppedFound = true;
        while (newDroppedFound) {
            newDroppedFound = false;
            for (int i = 0; i < bricks.size(); ++i) {
                if (parents[i].empty() || dropped.count(i) > 0) continue;
                bool hasNonDroppedParents = false;
                for (auto parent : parents[i]) {
                    if (dropped.count(parent) == 0) {
                        hasNonDroppedParents = true;
                        break;
                    }
                }
                if (!hasNonDroppedParents) {
                    dropped.insert(i);
                    newDroppedFound = true;
                }
            }
        }

        return dropped.size() - 1;
    };

    vint results(bricks.size());
    for (int i = 0; i < bricks.size(); ++i) {
        results[i] = BFS(i);
//        std::cout << i << " " << results[i] << std::endl;
    }

    std::cout << std::accumulate(results.begin(), results.end(), 0) << std::endl;

    return 0;
}
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
static const std::array DIRS = {LEFT, UP, RIGHT, DOWN};
static const auto NDIRS = DIRS.size();
using Point = std::pair<int, int>;
static const std::array<Point, NDIRS> DELTAS = {
        Point{0, -1}, // LEFT
        Point{-1, 0}, // UP
        Point{0, 1}, // RIGHT
        Point{1, 0}, // DOWN
};


using Edge = std::pair<size_t, Point>;
using Cost = int64_t;
const auto MAX_COST = std::numeric_limits<Cost>::max();
// point, kdir
using State = std::pair<Point, int>;
using CostState = std::pair<Cost, State>;

static Point addPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static auto readInput(std::istream &fin) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    const auto findPos = [&lines](char c) -> Point {
        Point outPos{-1, -1};
        for (int i = 0; i < lines.size(); ++i) {
            for (int j = 0; j < lines[0].size(); ++j) {
                if (lines[i][j] == c) {
                    outPos = Point{i, j};
                    lines[i][j] = '.';
                    break;
                }
            }
        }
        assert(outPos.first >= 0 && outPos.second >= 0);
        return outPos;
    };

    const auto startPos = findPos('S');
    const auto finishPos = findPos('E');
    return std::make_tuple(lines, startPos, finishPos);
}

static auto getEdges(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    // kdir, whereTo
    std::map<Point, std::vector<Edge>> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '.') {
                Point p{i, j};
                for (size_t k = 0; k < NDIRS; ++k) {
                    auto pNext = addPoint(p, DELTAS[k]);
                    if (isGoodPoint(pNext, n, m) && lines[pNext.first][pNext.second] == '.') {
                        edges[p].emplace_back(k, pNext);
                    }
                }
            }
        }
    }
    return edges;
}

// returns reachCost, ancestors
static auto dijkstra(const std::map<Point, std::vector<Edge>> &edges, Point startPos) {
    std::set<CostState> q;

    auto startKdirIt = std::find(DIRS.begin(), DIRS.end(), RIGHT);
    const auto startKdir = static_cast<int>(std::distance(DIRS.begin(), startKdirIt));
    assert(startKdir < NDIRS);
    auto startState = std::make_pair(startPos, startKdir);
    q.emplace(0, startState);

    // point, dir -> cost
    std::map<State, Cost> reachCost;
    reachCost[startState] = 0;

    std::map<State, std::vector<State>> ancestors;

    while (!q.empty()) {
        auto [cost, state] = *q.begin();
        q.erase(q.begin());
        auto [p, kdir] = state;
        assert(cost != MAX_COST);

        if (edges.count(p) == 0) continue;
        const auto &nextEdges = edges.at(p);
        for (auto [kdirNext, pNext]: nextEdges) {
            int rotateCost;
            if (kdirNext == kdir) {
                rotateCost = 0;
            } else if (kdirNext == (kdir + 2) % NDIRS) {
                rotateCost = 2 * 1000;
            } else {
                rotateCost = 1000;
            }
            const int moveCost = 1 + rotateCost;

            auto nextState = std::make_pair(pNext, kdirNext);
            auto nextStateCurrentCost = reachCost.count(nextState) ? reachCost[nextState] : MAX_COST;
            auto nextStateNewCost = cost + moveCost;

            if (nextStateNewCost < nextStateCurrentCost) {
                reachCost[nextState] = nextStateNewCost;
                q.emplace(nextStateNewCost, nextState);
                ancestors[nextState].clear();
                ancestors[nextState].push_back(state);
            } else if (nextStateNewCost == nextStateCurrentCost) {
                ancestors[nextState].push_back(state);
            }
        }
    }

    return std::make_tuple(reachCost, ancestors);
}

static auto calculateBestCost(const std::map<State, Cost> &reachCost, Point finishPos) {
    Cost bestCost = MAX_COST;
    for (int k = 0; k < NDIRS; ++k) {
        auto finishState = std::make_pair(finishPos, k);
        if (reachCost.count(finishState)) {
            bestCost = std::min(bestCost, reachCost.at(finishState));
        }
    }
    assert(bestCost != MAX_COST);
    return bestCost;
}

static auto star1(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    auto [lines, startPos, finishPos] = readInput(fin);

    auto edges = getEdges(lines);

    auto [reachCost, _] = dijkstra(edges, startPos);

    return calculateBestCost(reachCost, finishPos);
}

static auto star2(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    auto [lines, startPos, finishPos] = readInput(fin);

    auto edges = getEdges(lines);

    auto [reachCost, ancestors] = dijkstra(edges, startPos);

    auto bestCost = calculateBestCost(reachCost, finishPos);

    std::set<Point> usedTiles;

    std::set<State> usedStates;
    std::queue<State> q;

    for (int k = 0; k < NDIRS; ++k) {
        auto finishState = std::make_pair(finishPos, k);
        if (reachCost.count(finishState) && reachCost.at(finishState) == bestCost) {
            q.push(finishState);
            usedStates.insert(finishState);
        }
    }

    while (!q.empty()) {
        auto state = q.front();
        q.pop();

        auto [p, cost] = state;
        usedTiles.insert(p);

        if (p == startPos) continue;
        for (auto prevState: ancestors.at(state)) {
            if (!usedStates.count(prevState)) {
                q.push(prevState);
                usedStates.insert(prevState);
            }
        }
    }
    return usedTiles.size();
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl; // 7036
    std::cout << star1("example_input2.txt") << std::endl; // 11048
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl; // 85480

    std::cout << star2("example_input1.txt") << std::endl; // 45
    std::cout << star2("example_input2.txt") << std::endl; // 64
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl; // 518
    return 0;
}

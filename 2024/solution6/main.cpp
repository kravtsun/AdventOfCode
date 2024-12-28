#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cassert>
#include <set>

using Point = std::pair<int, int>;

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static Point addPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static Point subtractPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}

static const auto LEFT = '<';
static const auto RIGHT = '>';
static const auto DOWN = 'v';
static const auto UP = '^';
static const std::array DIRS = {LEFT, UP, RIGHT, DOWN};
static const auto NDIRS = DIRS.size();

static const std::array<Point, 4> DELTAS = {
        Point{0, -1}, // LEFT
        Point{-1, 0}, // UP
        Point{0, 1},  // RIGHT
        Point{1, 0},  // DOWN
};

static auto getDeltaIndex(const char delta) {
    auto distance = std::distance(DIRS.begin(), std::find(DIRS.begin(), DIRS.end(), delta));
    if (distance < 0 || distance >= DIRS.size()) {
        throw std::runtime_error("Delta error");
    }
    return static_cast<size_t>(distance);
}

static auto readLines(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    Point start;
    for (int i = 0; i < lines.size(); ++i) {
        bool found = false;
        for (auto d: DIRS) {
            if (const auto pos = lines[i].find(d); pos != std::string::npos) {
                start.first = i;
                start.second = static_cast<int>(pos);
                found = true;
                break;
            }
        }
        if (found) break;
    }
    return std::make_tuple(lines, start);
}

static auto star1(const std::string &filepath) {
    auto [lines, start] = readLines(filepath);
    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());

    auto p = start;
    const auto startDelta = lines[p.first][p.second];
    auto kdir = getDeltaIndex(startDelta);
    auto delta = DELTAS.at(kdir);

    while (true) {
        lines[p.first][p.second] = 'X';
        Point next{p.first + delta.first, p.second + delta.second};
        if (isGoodPoint(next, n, m)) {
            if (lines[next.first][next.second] == '#') {
                kdir = (kdir + 1) % NDIRS;
                delta = DELTAS.at(kdir);
                next = addPoint(p, delta);
            }
            p = next;
        } else {
            break;
        }
    }

    size_t result = 0;
    for (int i = 0; i < n; ++i) {
        result += std::count(lines[i].begin(), lines[i].end(), 'X');
    }
    return result;
}

struct State {
    Point p;
    size_t dir{};

    friend bool operator==(const State &lhs, const State &rhs) {
        return lhs.p == rhs.p && lhs.dir == rhs.dir;
    }

    friend bool operator!=(const State &lhs, const State &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const State &lhs, const State &rhs) {
        if (lhs.p != rhs.p)
            return lhs.p < rhs.p;
        if (lhs.dir != rhs.dir) {
            return lhs.dir < rhs.dir;
        }
        return false;
    }
};

const Point ESCAPING_POINT{-1, -1}; // This way we denote point of escaping.
const State ESCAPING_STATE{ESCAPING_POINT, 0};
const Point INVALID_POINT{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
const State INVALID_STATE{INVALID_POINT, 0};

// dirs, y-coordinate (Point::first), x-coordinate (Point::second)
using States = std::vector<std::vector<std::vector<State>>>;

static auto createGraph(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    // [NDIRS][n][m] -> the closest point by given direction by first index
    States nextStates(NDIRS, std::vector<std::vector<State>>(n, std::vector<State>(m, INVALID_STATE)));

    const auto isFreePoint = [&lines](const Point &p, const int n, const int m) {
        return isGoodPoint(p, n, m) && lines[p.first][p.second] == '.';
    };

    for (size_t d = 0; d < NDIRS; ++d) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (lines[i][j] == '.' && nextStates[d][i][j] == INVALID_STATE) {
                    auto delta = DELTAS.at(d);
                    Point startPoint{i, j};
                    Point p = startPoint;
                    for (; isFreePoint(p, n, m) && nextStates[d][p.first][p.second] == INVALID_STATE; p = addPoint(p,
                                                                                                                   delta));
                    // last point when the loop above ended
                    // - before we hit the field's borders, '#' or already visited cell
                    auto lastPoint = p;

                    State nextState;
                    if (!isGoodPoint(p, n, m)) {
                        // We hit the field's borders
                        nextState = ESCAPING_STATE;
                    } else if (lines[p.first][p.second] == '#') {
                        Point finishPoint = subtractPoint(p, delta);
                        nextState = State{finishPoint, (d + 1) % NDIRS};
                    } else {
                        assert(lines[p.first][p.second] == '.');
                        nextState = nextStates[d][p.first][p.second];
                        assert(nextState != INVALID_STATE);
                    }

                    for (p = startPoint; p != lastPoint; p = addPoint(p, delta)) {
                        nextStates[d][p.first][p.second] = nextState;
                    }
                }
            }
        }
    }
    return nextStates;
}

// returns set of updates to nextStates
static auto putObstacle(int n, int m, const States &nextStates, const Point &obstacle) {
    const auto isFreePointState = [&nextStates](size_t kdir, const Point &p, const int n, const int m) {
        return isGoodPoint(p, n, m) && nextStates[kdir][p.first][p.second] != INVALID_STATE;
    };
    std::map<State, State> updates;
    for (size_t kdir = 0; kdir < NDIRS; ++kdir) {
        updates[State{obstacle, kdir}] = INVALID_STATE;

        auto delta = DELTAS.at(kdir);
        auto rotateKDir = (kdir + 1) % NDIRS;
        auto rotatePoint = subtractPoint(obstacle, delta);
        if (!isGoodPoint(rotatePoint, n, m)) continue;

        State shouldNowGo{rotatePoint, rotateKDir};
        for (auto p = rotatePoint; isFreePointState(kdir, p, n, m); p = subtractPoint(p, delta)) {
            updates[State{p, kdir}] = shouldNowGo;
        }
    }
    return updates;
}

static bool
hasLoop(const States &nextStates, const std::map<State, State> &nextStatesUpdates, const State &startState) {
    const auto getNextState = [&nextStates, &nextStatesUpdates](const State &state) {
        return nextStatesUpdates.count(state) ? nextStatesUpdates.at(state)
                                              : nextStates[state.dir][state.p.first][state.p.second];
    };
    std::set<State> used;
    for (State state = startState; state != ESCAPING_STATE; state = getNextState(state)) {
        assert(nextStates[state.dir][state.p.first][state.p.second] != INVALID_STATE);
        if (used.count(state)) {
            return true;
        }
        used.insert(state);
    }
    return false;
}

static auto star2(const std::string &filepath) {
    auto [lines, start] = readLines(filepath);
    // build graph
    // each point around #, start point ^ should be connected
    // by placing '#' in a point '.' we need to introduce a new set of connected edges
    const auto startDelta = lines[start.first][start.second];
    const auto kdir = getDeltaIndex(startDelta);
    assert(kdir >= 0 && kdir < DIRS.size());
    lines[start.first][start.second] = '.';

    const auto nextStates = createGraph(lines);
    const State startState{start, static_cast<size_t>(kdir)};

    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());
    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '.' && Point{i, j} != start) {
                auto nextStatesUpdates = putObstacle(n, m, nextStates, Point{i, j});
                if (hasLoop(nextStates, nextStatesUpdates, startState)) {
                    result++;
                }
            }
        }
    }
    return result;
}

int main() {
    std::cout << star1("example_input.txt") << std::endl; // 41
    std::cout << star1("input.txt") << std::endl; // 4374
    std::cout << star2("example_input.txt") << std::endl; // 6
    std::cout << star2("input.txt") << std::endl; // 1705
    return 0;
}

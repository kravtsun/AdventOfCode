#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cassert>
#include <set>
#include <unordered_set>

using pii = std::pair<int, int>;

static bool isGoodCoordinate(int x, int n) {
    return 0 <= x && x < n;
}

static bool isGoodPoint(pii p, int n, int m) {
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static pii addPoint(const pii &lhs, const pii &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static pii subtractPoint(const pii &lhs, const pii &rhs) {
    return std::make_pair(lhs.first - rhs.first, lhs.second - rhs.second);
}

static const auto LEFT = '<';
static const auto RIGHT = '>';
static const auto DOWN = 'v';
static const auto UP = '^';
static const std::array dirs = {LEFT, UP, RIGHT, DOWN};
static const auto ndirs = static_cast<int>(dirs.size());

static const std::map<char, pii> dirToDelta = {
        {LEFT,  {0,  -1}},
        {RIGHT, {0,  1}},
        {UP,    {-1, 0}},
        {DOWN,  {1,  0}},
};

// TODO remove
static void printField(const std::vector<std::string> &lines) {
    for (const auto &l: lines) {
        std::cout << l << std::endl;
    }
}

static auto readLines(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    pii start;
    for (int i = 0; i < lines.size(); ++i) {
        bool found = false;
        for (auto d: dirs) {
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

static int star1(std::vector<std::string> lines, pii start) {
    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());

    auto p = start;
    const auto startPose = lines[p.first][p.second];
    auto ipose = std::distance(dirs.begin(), std::find(dirs.begin(), dirs.end(), startPose));

    auto delta = dirToDelta.at(dirs[ipose]);

    while (true) {
        lines[p.first][p.second] = 'X';
        pii next{p.first + delta.first, p.second + delta.second};
        if (isGoodCoordinate(next.first, n) && isGoodCoordinate(next.second, m)) {
            if (lines[next.first][next.second] == '#') {
                ipose = (ipose + 1) % ndirs;
                delta = dirToDelta.at(dirs[ipose]);
                next = addPoint(p, delta);
            }

            p = next;
        } else {
            break;
        }
    }

    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            result += lines[i][j] == 'X';
        }
    }
    return result;
}

static auto star1(const std::string &filepath) {
    auto [lines, start] = readLines(filepath);
    return star1(lines, start);
}

struct State {
    pii p;
    int dir{};

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

const pii ESCAPING_POINT{-1, -1}; // This way we denote point of escaping.
const State ESCAPING_STATE{ESCAPING_POINT, -1};
const pii INVALID_POINT{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
const State INVALID_STATE{INVALID_POINT, -1};

using States = std::vector<std::vector<std::vector<State>>>;

static auto createGraph(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    // [ndirs][n][m] -> the closest point by given direction by first index
    States nextStates(ndirs, std::vector<std::vector<State>>(n, std::vector<State>(m, INVALID_STATE)));

    const auto isFreePoint = [&lines](const pii &p, const int n, const int m) {
        return isGoodPoint(p, n, m) && lines[p.first][p.second] == '.';
    };

    for (int d = 0; d < ndirs; ++d) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (lines[i][j] == '.' && nextStates[d][i][j] == INVALID_STATE) {
                    auto delta = dirToDelta.at(dirs[d]);
                    pii startPoint{i, j};
                    pii p = startPoint;
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
                        pii finishPoint = subtractPoint(p, delta);
                        nextState = State{finishPoint, (d + 1) % ndirs};
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

static auto putObstacle(int n, int m, States nextStates, pii obstacle) {
    const auto isFreePointState = [&nextStates](int kdir, const pii &p, const int n, const int m) {
        return isGoodPoint(p, n, m) && nextStates[kdir][p.first][p.second] != INVALID_STATE;
    };
    for (int kdir = 0; kdir < ndirs; ++kdir) {
        nextStates[kdir][obstacle.first][obstacle.second] = INVALID_STATE;

        auto delta = dirToDelta.at(dirs[kdir]);
        auto rotateKDir = (kdir + 1) % ndirs;
        auto rotatePoint = subtractPoint(obstacle, delta);
        if (!isGoodPoint(rotatePoint, n, m)) continue;

        State shouldNowGo{rotatePoint, rotateKDir};
        for (auto p = rotatePoint; isFreePointState(kdir, p, n, m); p = subtractPoint(p, delta)) {
            nextStates[kdir][p.first][p.second] = shouldNowGo;
        }
    }
    return nextStates;
}

static bool hasLoop(const States &nextStates, const State &startState) {
    std::set<State> used;
    for (State state = startState;
         state != ESCAPING_STATE; state = nextStates[state.dir][state.p.first][state.p.second]) {
        assert(nextStates[state.dir][state.p.first][state.p.second] != INVALID_STATE);
        if (used.count(state)) {
            return true;
        }
        used.insert(state);
    }
    return false;
}

static int star2(std::vector<std::string> lines, pii start) {
    // build graph
    // each point around #, start point ^ should be connected
    // by placing '#' in a point '.' we need to introduce a new set of connected edges
    const auto startDir = lines[start.first][start.second];
    const auto dir = static_cast<int>(std::distance(dirs.begin(), std::find(dirs.begin(), dirs.end(), startDir)));
    assert(lines[start.first][start.second] == UP);
    lines[start.first][start.second] = '.';
    const State startState{start, dir};

    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());

    const auto nextStates = createGraph(lines);

    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == '.' && pii{i, j} != start) {
                auto currentNextStates = putObstacle(n, m, nextStates, pii{i, j});
                if (hasLoop(currentNextStates, startState)) {
                    result++;
                }
            }
        }
    }
    return result;
}

// 6, 1705
static auto star2(const std::string &filepath) {
    auto [lines, start] = readLines(filepath);
    return star2(lines, start);
}

int main() {
//    std::cout << star1("example_input.txt") << std::endl;
//    std::cout << star1("input.txt") << std::endl;

//    std::cout << star2("example_input.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;
    return 0;
}

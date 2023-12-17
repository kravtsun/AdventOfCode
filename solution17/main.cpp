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


using int64 = uint64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using vpii = std::vector<pii>;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

vs readField() {
    std::string line;
    vs field;
    while (std::getline(std::cin, line)) {
        field.push_back(line);
    }
    return field;
}

void printField(const vs &field) {
    for (const auto &f: field) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
}

static constexpr pii speedLeft{0, -1};
static constexpr pii speedRight{0, 1};
static constexpr pii speedUp{-1, 0};
static constexpr pii speedDown{+1, 0};

constexpr pii reverseSpeed(const pii &speed) {
    if (speed == speedUp) {
        return speedDown;
    } else if (speed == speedDown) {
        return speedUp;
    } else if (speed == speedLeft) {
        return speedRight;
    } else if (speed == speedRight) {
        return speedLeft;
    } else if (speed == pii(0, 0)) {
        return speed;
    } else {
        assert(false);
    }
}

static inline std::string speedString(const pii &speed) {
    if (speed == speedUp) {
        return "UP";
    } else if (speed == speedDown) {
        return "DOWN";
    } else if (speed == speedLeft) {
        return "LEFT";
    } else if (speed == speedRight) {
        return "RIGHT";
    } else if (speed == pii(0, 0)) {
        return "STOP";
    } else {
        assert(false);
    }
}

pii move(const pii &p, const pii &speed, int times) {
    return pii{p.first + times * speed.first, p.second + times * speed.second};
};

constexpr bool checkPointInBounds(const pii &p, const int height, const int width) {
//        return (!(p.first < 0 || p.first >= height || p.second < 0 || p.second >= width));
    return p.first >= 0 && p.first < height && p.second >= 0 && p.second < width;
};

struct State {
    pii point; // (i, j)
    pii speed; // speed i, speed j
    int heatLoss;

    State();

    State(pii point, pii speed, int heatLoss)
            : point(point), speed(speed), heatLoss(heatLoss) {}

    friend bool operator==(const State &lhs, const State &rhs) {
        return lhs.point == rhs.point &&
               lhs.speed == rhs.speed &&
               lhs.heatLoss == rhs.heatLoss;
    }

    friend bool operator!=(const State &lhs, const State &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const State &lhs, const State &rhs) {
        if (lhs.point != rhs.point) return lhs.point < rhs.point;
        if (lhs.speed != rhs.speed) return lhs.speed < rhs.speed;
        if (lhs.heatLoss != rhs.heatLoss) return lhs.heatLoss < rhs.heatLoss;
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, const State &state) {
        os << "(" << state.point.first << ", " << state.point.second << "): ";
//        os << state.speed.first << ", " << state.speed.second;
        os << speedString(state.speed);
        os << "heatLoss: " << state.heatLoss;
        return os;
    }
};

static const State INVALID_STATE{pii(-1, -1), pii(0, 0), -1};

State::State() {
    *this = INVALID_STATE;
}

// First star: 1, 3
const int MIN_MOVES_SAME_DIR = 1;
const int MAX_MOVES_SAME_DIR = 3;

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    auto field = readField();
    const int height = field.size();
    const int width = field[0].size();

    const int MAX_POSSIBLE_HEATLOSS = height * width * 10;

    const auto getHeatLoss = [&field](const pii &p) -> int {
        return static_cast<int>(field[p.first][p.second] - '0');
    };

    std::map<pii, std::vector<State>> m;
    const auto dstPoint = pii(height - 1, width - 1);
    int bestHeatloss = MAX_POSSIBLE_HEATLOSS;

    const vpii possibleSpeeds{speedLeft, speedRight, speedUp, speedDown};

    constexpr auto findBetter = [](const std::vector<State> &olderStates, const State &state, bool strict) -> bool {
        return std::any_of(olderStates.begin(), olderStates.end(), [&](const State &oldState) -> bool {
            assert(oldState.point == state.point);
            if (!strict && oldState == state) return true;
            if (oldState.speed == state.speed && oldState.heatLoss < state.heatLoss) {
                return true;
            }
            return false;
        });
    };

    State initialState{pii(0, 0), pii(0, 0), getHeatLoss(pii(0, 0))};

    std::queue<State> q;
    q.push(initialState);
    m[initialState.point].push_back(initialState);
    while (!q.empty()) {
        const State state = q.front();
        q.pop();

        if (findBetter(m[state.point], state, true)) continue;

        for (auto speed: possibleSpeeds) {
            if (speed == state.speed || reverseSpeed(speed) == state.speed) continue;

            auto firstNewPoint = move(state.point, speed, MIN_MOVES_SAME_DIR);
            if (!checkPointInBounds(firstNewPoint, height, width)) continue;

            int heatLoss = state.heatLoss;

            for (int i = 1; i < MIN_MOVES_SAME_DIR; ++i) {
                auto newPoint = move(state.point, speed, i);
                heatLoss += getHeatLoss(newPoint);
            }

            for (int i = MIN_MOVES_SAME_DIR; i <= MAX_MOVES_SAME_DIR; ++i) {
                auto newPoint = move(state.point, speed, i);
                if (!checkPointInBounds(newPoint, height, width)) break;
                heatLoss += getHeatLoss(newPoint);
                if (heatLoss > bestHeatloss) break;

                if (newPoint == dstPoint && heatLoss < bestHeatloss) {
                    bestHeatloss = heatLoss;
                }

                State newState;
                newState.point = newPoint;
                newState.speed = speed;
                newState.heatLoss = heatLoss;
                auto &newerStates = m[newPoint];

                if (!findBetter(newerStates, newState, false)) {
                    newerStates.push_back(newState);
                    q.push(newState);
                }
            }
        }
    }

    std::cout << bestHeatloss - getHeatLoss(pii(0, 0)) << std::endl;
    return 0;
}
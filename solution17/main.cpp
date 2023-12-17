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

static const pii speedLeft{0, -1};
static const pii speedRight{0, 1};
static const pii speedUp{-1, 0};
static const pii speedDown{+1, 0};

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

pii move(const pii &p, const pii &speed) {
    return pii{p.first + speed.first, p.second + speed.second};
};

struct State {
    pii point; // (i, j)
    pii speed; // speed i, speed j
    int movesLeft;
    int heatLoss;

    State();

    State(pii point, pii speed, int movesLeft, int heatLoss)
            : point(point), speed(speed), movesLeft(movesLeft), heatLoss(heatLoss) {}

    friend bool operator==(const State &lhs, const State &rhs) {
        return lhs.point == rhs.point &&
               lhs.speed == rhs.speed &&
               lhs.movesLeft == rhs.movesLeft &&
               lhs.heatLoss == rhs.heatLoss;
    }

    friend bool operator!=(const State &lhs, const State &rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const State &lhs, const State &rhs) {
        if (lhs.point != rhs.point) return lhs.point < rhs.point;
        if (lhs.speed != rhs.speed) return lhs.speed < rhs.speed;
        if (lhs.movesLeft != rhs.movesLeft) return lhs.movesLeft < rhs.movesLeft;
        if (lhs.heatLoss != rhs.heatLoss) return lhs.heatLoss < rhs.heatLoss;
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, const State &state) {
        os << "(" << state.point.first << ", " << state.point.second << "): ";
//        os << state.speed.first << ", " << state.speed.second;
        os << speedString(state.speed);
        os << " (" << state.movesLeft << ") ";
        os << "heatLoss: " << state.heatLoss;
        return os;
    }
};

static const State INVALID_STATE{pii(-1, -1), pii(0, 0), -1, -1};

State::State() {
    *this = INVALID_STATE;
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    auto field = readField();
    const int height = field.size();
    const int width = field[0].size();

    const auto checkPointInBounds = [height, width](const pii &p) -> bool {
//        return (!(p.first < 0 || p.first >= height || p.second < 0 || p.second >= width));
        return p.first >= 0 && p.first < height && p.second >= 0 && p.second < width;
    };

    const auto getHeatLoss = [&field](const pii &p) -> int {
        return static_cast<int>(field[p.first][p.second] - '0');
    };

    std::map<pii, std::vector<State>> m;

    const vpii possibleSpeeds{speedLeft, speedRight, speedUp, speedDown};

    const int MAX_MOVES_SAME_DIR = 3;

    constexpr auto findBetter = [](const std::vector<State> &olderStates, const State &state) -> bool {
        return std::any_of(olderStates.begin(), olderStates.end(), [&state](const State &oldState) -> bool {
            assert(oldState.point == state.point);
            if (oldState.speed == state.speed && oldState.movesLeft >= state.movesLeft &&
                oldState.heatLoss <= state.heatLoss) {
                return true;
            }
            return false;
        });
    };

    State initialState{pii(0, 0), pii(0, 0), 0, getHeatLoss(pii(0, 0))};

    std::queue<State> q;
    q.push(initialState);
    m[initialState.point].push_back(initialState);
    std::map<State, State> previousStates;
    previousStates[initialState] = INVALID_STATE;
    while (!q.empty()) {
        const State state = q.front();
        q.pop();

        for (auto speed: possibleSpeeds) {
            const auto newPoint = move(state.point, speed);
            if (move(newPoint, state.speed) == state.point) continue;
            if (!checkPointInBounds(newPoint)) continue;
            const auto newHeatLoss = getHeatLoss(newPoint);
            State newState{newPoint, speed, -1, state.heatLoss + newHeatLoss};

            if (speed == state.speed) {
                if (state.movesLeft <= 0) continue;
                newState.movesLeft = state.movesLeft - 1;
            } else {
                newState.movesLeft = MAX_MOVES_SAME_DIR - 1;
            }

            auto &newerStates = m[newPoint];
            if (!findBetter(newerStates, newState)) {
                previousStates[newState] = state;
                newerStates.push_back(newState);
                q.push(newState);
            }
        }
    }

    const auto dstPoint = pii(height - 1, width - 1);
    assert(m.count(dstPoint));

//    std::vector<State> path;

    int result = height * width * 10;
    for (auto state: m.at(dstPoint)) {
        if (state.heatLoss < result) {
            result = state.heatLoss;
//            path.clear();
//            for (auto s = state; s != INVALID_STATE; s = previousStates[s]) {
//                path.push_back(s);
//            }
        }
        result = std::min(result, state.heatLoss);
    }

//    std::reverse(path.begin(), path.end());
//    for (auto s: path) {
//        std::cout << s << std::endl;
//    }

    std::cout << result - getHeatLoss(pii(0, 0)) << std::endl;

//    const vs sampleField = {
//            "2>>34^>>>1323",
//            "32v>>>35v5623",
//            "32552456v>>54",
//            "3446585845v52",
//            "4546657867v>6",
//            "14385987984v4",
//            "44578769877v6",
//            "36378779796v>",
//            "465496798688v",
//            "456467998645v",
//            "12246868655<v",
//            "25465488877v5",
//            "43226746555v>",
//    };
//
//    assert(sampleField.size() == field.size() && sampleField[0].size() == field[0].size());
//    int sampleResult = 0;
//    for (int i = 0; i < height; ++i) {
//        for (int j = 0; j < width; ++j) {
//
//            const std::string directionCharacters = "><^v";
//            const auto sampleChar = sampleField[i][j];
//            const pii p{i, j};
//            if (directionCharacters.find(sampleChar) != std::string::npos) {
//                sampleResult += getHeatLoss(p);
////                pii speed;
////                if (sampleChar == '^') {
////                    speed = speedUp;
////                } else if (sampleChar == 'v') {
////                    speed = speedDown;
////                } else if (sampleChar == '<') {
////                    speed = speedLeft;
////                } else if (sampleChar == '>') {
////                    speed = speedRight;
////                } else {
////                    assert(false);
////                }
////
////                State sampleState{p, speed, };
////
////                assert(std::find(m[p].begin(), m[p].end(), State{});
//            }
//        }
//    }
//
//    std::cout << "sampleResult=" << sampleResult << std::endl;

    return 0;
}
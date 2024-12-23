#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <array>
#include <map>
#include <set>
#include <algorithm>

static const auto LEFT = '<';
static const auto RIGHT = '>';
static const auto DOWN = 'v';
static const auto UP = '^';
using pii = std::pair<int, int>;
const pii INVALID_POINT{-1, -1};

using Keypad = std::vector<std::string>;
static const Keypad NUMERIC_KEYPAD = {
        "789",
        "456",
        "123",
        " 0A",
};

static const Keypad DIRECTIONAL_KEYPAD = {
        " ^A",
        "<v>",
};

static auto getSymbolPosition(const Keypad &keypad, const char symbol) {
    pii symbolPos;
    for (int i = 0; i < keypad.size(); ++i) {
        if (auto pos = keypad[i].find(symbol); pos != std::string::npos) {
            symbolPos = pii{i, static_cast<int>(pos)};
        }
    }
    assert(symbolPos != INVALID_POINT);
    return symbolPos;
}

static auto possibleMovements(const pii &spacePos, const pii &pFrom, const pii &pTo) {
    auto verticalMovement = std::string(std::abs(pFrom.first - pTo.first), pTo.first > pFrom.first ? DOWN : UP);
    auto horizontalMovement = std::string(std::abs(pFrom.second - pTo.second), pTo.second > pFrom.second ? RIGHT : LEFT);

    std::vector<std::string> movements;
    std::string wholeMovement;
    if (pFrom.first == spacePos.first && pTo.second == spacePos.second) {
        assert(pTo.first != pFrom.first && pTo.second != pFrom.second);
        movements.push_back(verticalMovement + horizontalMovement);

        auto horizontalMove = horizontalMovement.back();
        auto verticalMove = verticalMovement.back();
        horizontalMovement.pop_back();
        verticalMovement.pop_back();
        auto aroundGapMovement = std::string(1, verticalMove) + std::string(1, horizontalMove);
        movements.push_back(horizontalMovement + aroundGapMovement + verticalMovement);
    } else if (pTo.first == spacePos.first && pFrom.second == spacePos.second) {
        assert(pTo.first != pFrom.first && pTo.second != pFrom.second);
        movements.push_back(horizontalMovement + verticalMovement);

        auto horizontalMove = horizontalMovement.back();
        auto verticalMove = verticalMovement.back();
        horizontalMovement.pop_back();
        verticalMovement.pop_back();
        auto aroundGapMovement = std::string(1, horizontalMove) + std::string(1, verticalMove);
        movements.push_back(verticalMovement + aroundGapMovement + horizontalMovement);
    } else {
        movements.push_back(horizontalMovement + verticalMovement);
        movements.push_back(verticalMovement + horizontalMovement);
    }
    if (movements.size() == 2 && movements.front() == movements.back()) {
        movements.pop_back();
    }
    std::transform(movements.begin(), movements.end(), movements.begin(), [](const std::string &movement) {
        return movement + "A";
    });
    return movements;
}

// Movement - a path represented as an array of moves (LEFT, UP, RIGHT, DOWN).
using Movement = std::string;
// represents ends of the movement - start and finish
using PointsPair = std::pair<pii, pii>;
using Movements = std::map<PointsPair, std::vector<Movement>>;

static auto calculateAllMovements(const pii &spacePos, const Keypad &keypad) {
    Movements result;
    for (int iFrom = 0; iFrom < keypad.size(); ++iFrom) {
        for (int jFrom = 0; jFrom < keypad[0].size(); ++jFrom) {
            pii pFrom{iFrom, jFrom};
            if (pFrom == spacePos) continue;
            for (int iTo = 0; iTo < keypad.size(); ++iTo) {
                for (int jTo = 0; jTo < keypad[0].size(); ++jTo) {
                    pii pTo{iTo, jTo};
                    if (pTo == spacePos) continue;

                    PointsPair pFromTo{pFrom, pTo};
                    if (pFrom == pTo) {
                        result[pFromTo] = {"A"};
                    } else {
                        result[pFromTo] = possibleMovements(spacePos, pFrom, pTo);
                    }
                }
            }

        }
    }
    return result;
}

template<bool OnlyStringSizeMatters>
struct KeypadState {
    pii lastPos;
    std::string bestStringSoFar;

    friend bool operator<(const KeypadState &lhs, const KeypadState &rhs) {
        if (lhs.lastPos != rhs.lastPos) {
            return lhs.lastPos < rhs.lastPos;
        }
        if constexpr (OnlyStringSizeMatters) {
            // We do not need to have all possible strings of given length leading to lastPos being as it is.
            if (lhs.bestStringSoFar.back() != rhs.bestStringSoFar.back()) {
                return lhs.bestStringSoFar.back() < rhs.bestStringSoFar.back();
            }
            return lhs.bestStringSoFar.size() < rhs.bestStringSoFar.size();
        } else {
            // We need to keep all the strings present, because the can influence movements on lower levels:
            // numeric -> directional1 -> directional2
            return lhs.bestStringSoFar < rhs.bestStringSoFar;
        }
    }
};

const auto SPACE_POS_NUMERIC = getSymbolPosition(NUMERIC_KEYPAD, ' ');
const auto SPACE_POS_DIRECTIONAL = getSymbolPosition(DIRECTIONAL_KEYPAD, ' ');

static auto star1(const std::string &filepath) {
    // pre-calculate all state transitions for robot before the numeric keypad
    // lastPos, string-increase
    const auto allNumericMovements = calculateAllMovements(SPACE_POS_NUMERIC, NUMERIC_KEYPAD);
    const auto allDirectionalMovements = calculateAllMovements(SPACE_POS_DIRECTIONAL, DIRECTIONAL_KEYPAD);

    using NumericKeypadState = KeypadState<false>;
    using Directional1KeypadState = KeypadState<false>;
    using Directional2KeypadState = KeypadState<true>;

    using AllKeypadsState = std::tuple<NumericKeypadState, Directional1KeypadState, Directional2KeypadState>;

    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;

    int result = 0;
    while (std::getline(fin, line)) {
        NumericKeypadState numericStartState{getSymbolPosition(NUMERIC_KEYPAD, 'A'), ""};
        Directional1KeypadState directional1StartState{getSymbolPosition(DIRECTIONAL_KEYPAD, 'A'), ""};
        Directional2KeypadState directional2StartState{getSymbolPosition(DIRECTIONAL_KEYPAD, 'A'), ""};
        std::set<AllKeypadsState> states;
        states.insert({numericStartState, directional1StartState, directional2StartState});

        for (auto numericCode: line) {
            std::set<AllKeypadsState> nextStates;
            auto numericPos = getSymbolPosition(NUMERIC_KEYPAD, numericCode);
            for (const auto &state: states) {
                const auto &[numericState, directional1State, directional2State] = state;
                auto numericMovements = allNumericMovements.at(std::make_pair(numericState.lastPos, numericPos));
                for (const auto &numericMovement: numericMovements) {
                    using DirectionalKeypadStates = std::tuple<Directional1KeypadState, Directional2KeypadState>;
                    std::set<DirectionalKeypadStates> currentDirectional12States{{directional1State, directional2State}};
                    for (auto numericMove: numericMovement) {
                        auto directional1Pos = getSymbolPosition(DIRECTIONAL_KEYPAD, numericMove);
                        std::set<DirectionalKeypadStates> nextDirectional12States;
                        for (const auto &[currentDirectional1State, currentDirectional2State]: currentDirectional12States) {
                            auto directional1Movements = allDirectionalMovements.at(std::make_pair(currentDirectional1State.lastPos, directional1Pos));
                            for (const auto &directional1Movement: directional1Movements) {
                                std::set<Directional2KeypadState> currentDirectional2States = {currentDirectional2State};
                                for (auto directional1Move : directional1Movement) {
                                    auto directional2Pos = getSymbolPosition(DIRECTIONAL_KEYPAD, directional1Move);
                                    std::set<Directional2KeypadState> nextDirectional2States;
                                    for (const auto& currentDirectional2InnerState : currentDirectional2States) {
                                        auto directional2Movements = allDirectionalMovements.at(std::make_pair(currentDirectional2InnerState.lastPos, directional2Pos));
                                        for (const auto& directional2Movement : directional2Movements) {
                                            Directional2KeypadState nextDirectional2State{directional2Pos, currentDirectional2InnerState.bestStringSoFar + directional2Movement};
                                            nextDirectional2States.insert(nextDirectional2State);
                                        }
                                    }
                                    currentDirectional2States = nextDirectional2States;
                                }
                                Directional1KeypadState newDirectional1State{directional1Pos, currentDirectional1State.bestStringSoFar + directional1Movement};
                                for (const auto& currentDirectional2InnerState : currentDirectional2States) {
                                    nextDirectional12States.emplace(newDirectional1State, currentDirectional2InnerState);
                                }
                            }
                        }
                        currentDirectional12States = nextDirectional12States;
                    }

                    NumericKeypadState newNumericState{numericPos, numericState.bestStringSoFar + numericMovement};
                    for (const auto &[currentDirectional1State, currentDirectional2State]: currentDirectional12States) {
                        nextStates.emplace(newNumericState, currentDirectional1State, currentDirectional2State);
                    }
                }
            }
            states = nextStates;
        }

        int bestResult = -1;
        std::string bestString;
        for (const auto &state: states) {
            const auto &[numericState, directional1State, directional2State] = state;
            if (bestResult == -1 || bestString.size() > directional2State.bestStringSoFar.size()) {
                bestString = directional2State.bestStringSoFar;
                bestResult = static_cast<int>(bestString.size());
            }
        }

        result += std::stoi(line.substr(0, line.size() - 1)) * bestResult;
    }
    return result;
}

int main() {
    std::cout << star1("example_input.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;
    return 0;
}

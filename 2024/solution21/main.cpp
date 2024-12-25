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

static const char SYMBOL_PUSH = 'A';
static const char SYMBOL_SPACE = ' ';

static auto possibleMovements(const pii &spacePos, const pii &pFrom, const pii &pTo) {
    auto verticalMovement = std::string(std::abs(pFrom.first - pTo.first), pTo.first > pFrom.first ? DOWN : UP);
    auto horizontalMovement = std::string(std::abs(pFrom.second - pTo.second),
                                          pTo.second > pFrom.second ? RIGHT : LEFT);

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
        return movement + std::string(1, SYMBOL_PUSH);
    });
    return movements;
}

// Movement - a path represented as an array of moves (LEFT, UP, RIGHT, DOWN).
using Movement = std::string;
// represents ends of the movement - start and finish
using PointsPair = std::pair<pii, pii>;

static auto calculateAllMovements(const pii &spacePos, const Keypad &keypad) {
    std::map<PointsPair, std::vector<Movement>> result;
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

static auto calculateAllSymbolPositions(const Keypad &keypad) {
    std::map<char, pii> result;
    for (int i = 0; i < keypad.size(); ++i) {
        for (int j = 0; j < keypad[i].size(); ++j) {
            result[keypad[i][j]] = pii{i, j};
        }
    }
    return result;
}

static std::map<std::tuple<char, char, int, int>, int64_t> memo;

static int64_t solveRecursive(char previousCode, char nextCode, int level, const int maxLevel) {
    static const auto numericSymbolPositions = calculateAllSymbolPositions(NUMERIC_KEYPAD);
    static const auto directionalSymbolPositions = calculateAllSymbolPositions(DIRECTIONAL_KEYPAD);
    static const auto allNumericMovements = calculateAllMovements(numericSymbolPositions.at(SYMBOL_SPACE),
                                                                  NUMERIC_KEYPAD);
    static const auto allDirectionalMovements = calculateAllMovements(directionalSymbolPositions.at(SYMBOL_SPACE),
                                                                      DIRECTIONAL_KEYPAD);

    const auto inputParameters = std::make_tuple(previousCode, nextCode, level, maxLevel);
    auto memoIt = memo.find(inputParameters);
    if (memoIt != memo.end()) {
        return memoIt->second;
    }

    const auto &symbolPositions = level == 0 ? numericSymbolPositions : directionalSymbolPositions;
    const PointsPair movementsPair{symbolPositions.at(previousCode), symbolPositions.at(nextCode)};
    const std::vector<std::string> &movements = level == 0 ?
                                                allNumericMovements.at(movementsPair) :
                                                allDirectionalMovements.at(movementsPair);

    int64_t bestMovementResult = std::numeric_limits<int64_t>::max();

    for (const auto &movement: movements) {
        int64_t currentMovementResult = 0;
        if (level == maxLevel) {
            currentMovementResult = static_cast<int>(movement.size());
            assert(movement.back() == SYMBOL_PUSH);
        } else {
            char previousMove = SYMBOL_PUSH;
            for (auto move: movement) {
                currentMovementResult += solveRecursive(previousMove, move, level + 1, maxLevel);
                previousMove = move;
            }
            assert(previousMove == SYMBOL_PUSH);
        }
        if (currentMovementResult < bestMovementResult) {
            bestMovementResult = currentMovementResult;
        }
    }
    memo.insert(memoIt, {inputParameters, bestMovementResult});
    return bestMovementResult;
}

static auto solve(const std::string &filepath, const int maxLevel) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;

    int64_t result = 0;
    while (std::getline(fin, line)) {
        int64_t lineResult = 0;
        char prevCode = SYMBOL_PUSH;
        for (auto code: line) {
            lineResult += solveRecursive(prevCode, code, 0, maxLevel);
            prevCode = code;
        }
        result += std::stoi(line.substr(0, line.size() - 1)) * lineResult;
    }
    return result;
}

static auto star1(const std::string &filepath) {
    return solve(filepath, 2);
}

static auto star2(const std::string &filepath) {
    return solve(filepath, 25);
}

int main() {
    std::cout << star1("example_input.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;

    return 0;
}

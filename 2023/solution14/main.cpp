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
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

#define EMPTY_BLOCK '.'
#define CIRCULAR 'O'
#define CUBIC '#'

void printField(const vs& field) {
    for (const auto&f : field) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
}

std::string fieldToString(const vs& field) {
    std::string s;
    for (const auto& f: field) {
        s += f;
    }
    return s;
}

vs rollNorth(vs field) {
    const int height = field.size();
    const int width = field[0].size();

    for (int j = 0; j < width; ++j) {
        for (int i = 0; i < height; ++i) {
            while (i < height && field[i][j] == CUBIC) i++;

            int startPos = i;
            int countCircular = 0;

            for (; i < height && field[i][j] != CUBIC; ++i) {
                countCircular += field[i][j] == CIRCULAR;
            }

            int limitPos = i;

            for (int ii = startPos; ii < limitPos; ++ii) {
                assert(countCircular >= 0);
                if (countCircular == 0) {
                    field[ii][j] = EMPTY_BLOCK;
                } else {
                    field[ii][j] = CIRCULAR;
                    countCircular--;
                }
            }
        }
    }
    return field;
}

[[nodiscard]] vs rotateCW(const vs& field) {
    const int height = field.size();
    const int width = field[0].size();
    vs newField(width, std::string(height, 0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
//            newField[width - j - 1][i] = field[i][j]; // CCW
            newField[j][height - i - 1] = field[i][j]; // CW
        }
    }
    return newField;
}

int totalLoad(const vs& field) {
    int result = 0;
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[0].size(); ++j) {
            if (field[i][j] == CIRCULAR) {
                result += field.size() - i;
            }
        }
    }
    return result;
}

int solve(vs field, int loops) {

    const auto singleLoop = [](vs& field) -> void {
        for (int j = 0; j < 4; ++j) {
            field = rollNorth(field);
            field = rotateCW(field);
        }
    };

    std::map<std::string, int> stringToLoop;
    for (int i = 0; i < loops; ++i) {
        singleLoop(field);

        const auto fieldString = fieldToString(field);
        if (stringToLoop.count(fieldString) > 0) {
            const int startLoop = stringToLoop.at(fieldString);
            const int loopLength = i - startLoop;
//            std::cout << "startLoop=" << startLoop << std::endl;
//            std::cout << "LoopLength=" << loopLength << std::endl;
            loops -= startLoop;
            loops %= loopLength;
            for (int j = 0; j < loops; ++j) {
//                std::cout << "j=" << j << std::endl;
//                std::cout << "totalLoad=" << totalLoad(field) << std::endl;
                singleLoop(field);
            }
            break;
        } else {
            stringToLoop[fieldString] = i;
        }
    }

    return totalLoad(field);
}

int main() {
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    vs field;
    std::string line;
    while (std::getline(std::cin, line)) {
        field.push_back(line);
    }

//    for (int i = 0; i < 20; i++) {
//        const auto result = solve(field, i);
//        std::cout << "i=" << i << ", " << result << std::endl;
//    }

    const auto result = solve(field, 1000000000 - 1);
    std::cout << result << std::endl;


    return 0;
}
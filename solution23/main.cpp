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

vs readField() {
    std::string line;
    vs field;
    while (std::getline(std::cin, line)) {
        field.push_back(line);
    }
    return field;
}

void printField(const vs& field) {
    for (const auto&f : field) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
}

static const pii speedLeft{0, -1};
static const pii speedRight{0, 1};
static const pii speedUp{-1, 0};
static const pii speedDown{+1, 0};

static const vpii speeds{speedLeft, speedRight, speedUp, speedDown};

pii move(const pii &p, const pii &speed, int times) {
    return pii{p.first + times * speed.first, p.second + times * speed.second};
};

constexpr bool checkPointInBounds(const pii &p, const int height, const int width) {
//        return (!(p.first < 0 || p.first >= height || p.second < 0 || p.second >= width));
    return p.first >= 0 && p.first < height && p.second >= 0 && p.second < width;
};

static const std::string speedChars = "<>^v";
pii charToSpeed(char c) {
    if (c == '>') return speedRight;
    else if (c == '<') return speedLeft;
    else if (c == '^') return speedUp;
    else if (c == 'v') return speedDown;
    else assert(false);
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
//    std::ifstream fin{WORKDIR "input_sample.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());


    auto field = readField();
    const int height = field.size();
    const int width = field[0].size();

    std::vector<vbool> used(height, vbool(width, false));

    int result = -1;

    vs bestField;

    std::function<void(pii, int)> DFS;
    DFS = [&](pii p, int currentResult) -> void {

        if (p.first == height - 1) {
            if (currentResult > result) {
                bestField = field;
                result = currentResult;
            }
        }

        auto originalC = field[p.first][p.second];
        auto c = field[p.first][p.second];
        // star 1
//        auto availableSpeeds = (speedChars.find(c) != std::string::npos) ? std::vector<pii>{charToSpeed(c)} : speeds;
        // star 2
        auto availableSpeeds = speeds;

        assert(!used.at(p.first).at(p.second));
        used.at(p.first).at(p.second) = true;
        field[p.first][p.second] = 'O';

        for (auto speed : availableSpeeds) {
            auto to = move(p, speed, 1);
            if (!checkPointInBounds(to, height, width)) continue;
            if (field.at(to.first).at(to.second) == '#') continue;
            if (used.at(to.first).at(to.second)) continue;
            DFS(to, currentResult + 1);
        }

        field[p.first][p.second] = originalC;
        used.at(p.first).at(p.second) = false;
    };

    int jStart = field[0].find('.');
    int jFinish = field.back().find('.');
    DFS(pii(0, jStart), 0);

    printField(bestField);

    std::cout << result << std::endl;

    return 0;
}
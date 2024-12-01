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
#include <memory>

using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
using vpii = std::vector<pii>;
using spii = std::set<pii>;
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

pii move(const pii &p, const pii &speed, int times) {
    return pii{p.first + times * speed.first, p.second + times * speed.second};
};

bool checkPointInBounds(const pii &p, const int height, const int width) {
//        return (!(p.first < 0 || p.first >= height || p.second < 0 || p.second >= width));
    return p.first >= 0 && p.first < height && p.second >= 0 && p.second < width;
}

pii mappedPoint(const pii& p, const int height, const int width) {
    const auto firstTimes = std::abs(p.first) / height + 3;
    const auto secondTimes = std::abs(p.second) / width + 3;
    return pii{(p.first + firstTimes * height) % height,
               (p.second + secondTimes * width) % width};
}

static const pii speedLeft{0, -1};
static const pii speedRight{0, 1};
static const pii speedUp{-1, 0};
static const pii speedDown{+1, 0};

static const vpii speeds{speedLeft, speedRight, speedUp, speedDown};

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    vs field = readField();
    const int height = field.size();
    const int width = field[0].size();

    pii startPoint{-1, -1};

    for (int i = 0; i < height; ++i) {
        auto j = field[i].find('S');
        if (j != std::string::npos) {
            startPoint = pii(i, j);
            field[i][j] = '.';
            break;
        }
    }

    // In exactly 6 steps, he can still reach 16 garden plots.
    //In exactly 10 steps, he can reach any of 50 garden plots.
    //In exactly 50 steps, he can reach 1594 garden plots.
    //In exactly 100 steps, he can reach 6536 garden plots.
    //In exactly 500 steps, he can reach 167004 garden plots.
    //In exactly 1000 steps, he can reach 668697 garden plots.
    //In exactly 5000 steps, he can reach 16733044 garden plots.

    spii points;
    points.insert(startPoint);
    for (int i = 0; i < 64; ++i) {
        spii nextPoints;
        for (auto p : points) {
            for (auto speed : speeds) {
                auto p1 = move(p, speed, 1);

                pii p2 = mappedPoint(p1, height, width);

                if (field[p2.first][p2.second] != '#') {
                    nextPoints.insert(p1);
                }
            }
        }
        std::swap(points, nextPoints);
    }

    std::cout << points.size() << std::endl;

    return 0;
}
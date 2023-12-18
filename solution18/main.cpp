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

enum class DIR {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

DIR charToDir(char c) {
    if (c == 'U') {
        return DIR::UP;
    } else if (c == 'D') {
        return DIR::DOWN;
    } else if (c == 'L') {
        return DIR::LEFT;
    } else if (c == 'R') {
        return DIR::RIGHT;
    } else {
        assert(false);
    }
}

static const pii speedLeft{0, -1};
static const pii speedRight{0, 1};
static const pii speedUp{-1, 0};
static const pii speedDown{+1, 0};


pii move(const pii &p, const pii &speed, int times = 1) {
    return pii{p.first + times * speed.first, p.second + times * speed.second};
};

pii dirToSpeed(DIR dir) {
    switch (dir) {
        case DIR::UP:
            return speedUp;
        case DIR::DOWN:
            return speedDown;
        case DIR::LEFT:
            return speedLeft;
        case DIR::RIGHT:
            return speedRight;
    }
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::vector<std::pair<DIR, int>> v;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        char dir;
        int num;
        lineStream >> dir >> num;
        v.emplace_back(charToDir(dir), num);
    }

    int imin = INT32_MAX;
    int imax = INT32_MIN;

    int jmin = INT32_MAX;
    int jmax = INT32_MIN;

    pii p{0, 0};
    for (auto [dir, num] : v) {
        auto speed = dirToSpeed(dir);
        p = move(p, speed, num);
        imin = std::min(imin, p.first);
        imax = std::max(imax, p.first);
        jmin = std::min(jmin, p.second);
        jmax = std::max(jmax, p.second);
    }

    const int width = jmax - jmin + 1;
    const int height = imax - imin + 1;

    vs field(height + 2);
    for (int i = 0; i < height + 2; ++i) {
        for (int j = 0; j < width + 2; ++j) {
            field[i] += '.';
        }
//        field[i].resize(width + 2);
    }

    p.first -= imin;
    p.second -= jmin;

    p.first += 1;
    p.second += 1;

    for (auto [dir, num] : v) {
        auto speed = dirToSpeed(dir);
        auto nextPoint = move(p, speed, num);

        auto [ifrom, jfrom] = p;
        auto [ito, jto] = nextPoint;
        if (ifrom > ito) {
            std::swap(ifrom, ito);
        }

        if (jfrom > jto) {
            std::swap(jfrom, jto);
        }

        for (int i = ifrom; i <= ito; ++i) {
            for (int j = jfrom; j <= jto; ++j) {
                field[i][j] = '#';
                assert(i >= 0 && i < field.size());
                assert(j >= 0 && j < field[0].size());
            }
        }
        p = nextPoint;
    }

    const auto paint = [&field](pii startPoint, char color) {
        // Paint points (BFS)
        std::queue<pii> q;
        q.emplace(startPoint.first, startPoint.second);

        while (!q.empty()) {
            auto v = q.front();
            q.pop();

            if (field[v.first][v.second] != '.') continue;
            field[v.first][v.second] = color;
            const int DX[] = {0, 0, -1, 1};
            const int DY[] = {-1, 1, 0, 0};
            for (int k = 0; k < 4; ++k) {
                const auto dx = DX[k];
                const auto dy = DY[k];
                const auto newY = v.first + dy;
                if (newY < 0 || newY >= field.size()) {
                    continue;
                }
                const auto newX = v.second + dx;
                if (newX < 0 || newX >= field[0].size()) {
                    continue;
                }
                q.emplace(newY, newX);
            }
        }
    };

    for (int i = 0; i < field.size(); ++i) {
        paint(pii{i, 0}, 'O');
        paint(pii{i, field[0].size() - 1}, 'O');
    }

    for (int j = 0; j < field[0].size(); ++j) {
        paint(pii{0, j}, 'O');
        paint(pii{field.size() - 1, j}, 'O');
    }
    printField(field);

    int result = 0;
    for (auto lf : field) {
        result += std::count(lf.begin(), lf.end(), '.');
        result += std::count(lf.begin(), lf.end(), '#');
    }

    std::cout << result << std::endl;

    return 0;
}
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


using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
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

char dirToChar(DIR dir) {
    switch (dir) {
        case DIR::UP:
            return 'U';
        case DIR::DOWN:
            return 'D';
        case DIR::LEFT:
            return 'L';
        case DIR::RIGHT:
            return 'R';
    }
}

static const pii speedLeft{0, -1};
static const pii speedRight{0, 1};
static const pii speedUp{-1, 0};
static const pii speedDown{+1, 0};


template<typename T>
std::pair<T, T> move(const std::pair<T, T> &p, const pii &speed, T times = 1) {
    return std::pair<T, T>{p.first + times * speed.first, p.second + times * speed.second};
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

int64 hexToNumber(const std::string& code) {
    int64 result = 0;
    for (auto c: code) {
        result *= 16;
        if (isdigit(c)) {
            result += c - '0';
        } else {
            result += 10 + c - 'a';
        }
    }
    return result;
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    vpii64 points;
    const pii64 startPoint{0, 0};
    pii64 p = startPoint;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        char dirChar;
        int64 num;
        lineStream >> dirChar >> num;
        char c;
        lineStream >> c;
        if (c == ' ') {
            lineStream >> c;
            assert(c == '(');
        }
        lineStream >> c;
        assert(c == '#');
        std::string code;
        for (int i = 0; i < 5; ++i) {
            lineStream >> c;
            code += c;
        }
        num = hexToNumber(code);
        lineStream >> dirChar;

        DIR dir;
        if (dirChar == '0') {
            dir = DIR::RIGHT;
        } else if (dirChar == '1') {
            dir = DIR::DOWN;
        } else if (dirChar == '2') {
            dir = DIR::LEFT;
        } else if (dirChar == '3') {
            dir = DIR::UP;
        } else {
            assert(false);
        }

//        std::cout << dirToChar(dir) << " " << num << std::endl;

        points.push_back(p);
        auto speed = dirToSpeed(dir);
        p = move<int64>(p, speed, num);
    }

    assert(p == startPoint);

    int64 minSecond = INT64_MAX;
    int64 minFirst = INT64_MAX;
    int64 maxSecond = INT64_MIN;
    int64 maxFirst = INT64_MIN;
    for (const auto& p : points) {
        minSecond = std::min(minSecond, p.second);
        minFirst = std::min(minFirst, p.first);
        maxSecond = std::max(maxSecond, p.second);
        maxFirst = std::max(maxFirst, p.first);
    }

    int64 perimeter = 0;
    int64 result = 0;
    for (int i = 0; i < points.size(); ++i) {
        const auto currentPoint = points[i];
        const auto nextPoint = i + 1 < points.size() ? points[i + 1] : points[0];
        if (currentPoint.first != nextPoint.first) {
            int64 newArea = (currentPoint.first - nextPoint.first) * currentPoint.second;
            result += newArea;
        }

        perimeter += std::abs(currentPoint.first - nextPoint.first);
        perimeter += std::abs(currentPoint.second - nextPoint.second);
    }
    assert(perimeter % 2 == 0);
    std::cout << std::abs(result) + perimeter / 2 + 1  << std::endl;

    return 0;
}
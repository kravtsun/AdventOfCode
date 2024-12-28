#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>

// x, y
using Point = std::pair<int, int>;

template<typename Token>
static void readToken(std::istream &is, const std::vector<Token> &expectedTokens) {
    Token token;
    is >> token;
    auto it = std::find(expectedTokens.begin(), expectedTokens.end(), token);
    if (it == expectedTokens.end()) {
        std::ostringstream os;
        os << "Read: " << token << ", Expected: ";
        for (const auto &t: expectedTokens) {
            os << t << ",";
        }
        throw std::runtime_error(os.str());
    }
}

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

using Robot = std::pair<Point, Point>;

static auto readRobots(const std::string &filepath, int firstDim, int secondDim) {
    std::ifstream fin{filepath};
    std::vector<Robot> robots;

    std::string line;

    while (std::getline(fin, line)) {
        std::istringstream is{line};
        readToken<char>(is, {'p'});
        readToken<char>(is, {'='});
        Point p, v;
        is >> p.first;
        readToken<char>(is, {','});
        is >> p.second;
        assert(isGoodPoint(p, firstDim, secondDim));

        readToken<char>(is, {'v'});
        readToken<char>(is, {'='});
        is >> v.first;
        readToken<char>(is, {','});
        is >> v.second;
        if (v.first < 0) v.first += firstDim;
        if (v.second < 0) v.second += secondDim;
        assert(isGoodPoint(v, firstDim, secondDim));

        robots.emplace_back(p, v);
    }

    return robots;
}

static int star1(const std::string &filepath, const int firstDim, const int secondDim) {
    auto robots = readRobots(filepath, firstDim, secondDim);
    int quadrantCounts[2][2] = {};

    for (auto [p, v]: robots) {
        p.first = (p.first + 100 * v.first) % firstDim;
        p.second = (p.second + 100 * v.second) % secondDim;
        if (p.first == firstDim / 2 || p.second == secondDim / 2) continue;
        quadrantCounts[p.first > firstDim / 2][p.second > secondDim / 2]++;
    }
    return quadrantCounts[0][0] * quadrantCounts[0][1] * quadrantCounts[1][0] * quadrantCounts[1][1];
}

static void star2(const std::string &filepath, const int firstDim, const int secondDim) {
    auto robots = readRobots("input.txt", firstDim, secondDim);
    for (int k = 0; k < 101 * 103; ++k) {
        std::vector<std::string> lines(secondDim, std::string(firstDim, '.'));
        for (auto [p, v]: robots) {
            Point newPoint{(p.first + k * v.first) % firstDim, (p.second + k * v.second) % secondDim};
            lines[newPoint.second][newPoint.first] = '#';
        }

        std::ofstream fout{"output_" + std::to_string(k) + ".txt"};
        for (const auto &line: lines) {
            fout << line << std::endl;
        }
    }
}

int main() {
    std::cout << star1("example_input.txt", 11, 7) << std::endl;
    std::cout << star1("input.txt", 101, 103) << std::endl;
    star2("input.txt", 101, 103); // next we search for the Christmas tree in generated files
    return 0;
}

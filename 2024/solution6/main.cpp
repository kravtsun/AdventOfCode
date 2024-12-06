#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cassert>
#include <set>
#include <unordered_set>

static const auto LEFT = '<';
static const auto RIGHT = '>';
static const auto DOWN = 'v';
static const auto UP = '^';
static const std::array dirs = {LEFT, UP, RIGHT, DOWN};
static const auto ndirs = static_cast<int>(dirs.size());
using pii = std::pair<int, int>;

static const std::map<char, pii> leads_where = {
        {LEFT, {0, -1}}, {RIGHT, {0, 1}}, {UP, {-1, 0}}, {DOWN, {1, 0}},
};

static bool isGoodCoordinate(int x, int n) {
    return 0 <= x && x < n;
}

static void print_field(const std::vector<std::string> &lines) {
    for (const auto &l : lines) {
        std::cout << l << std::endl;
    }
}

static int star1(std::vector<std::string> lines, pii start) {
    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());

    auto p = start;
    const auto startPose = lines[p.first][p.second];
    const auto startPoseIt = std::find(dirs.begin(), dirs.end(), startPose);
    auto ipose = std::distance(dirs.begin(), startPoseIt);

    auto where = leads_where.at(dirs[ipose]);

    while (true) {
        lines[p.first][p.second] = 'X';
        pii next{p.first + where.first, p.second + where.second};
        if (isGoodCoordinate(next.first, n) && isGoodCoordinate(next.second, m)) {
            if (lines[next.first][next.second] == '#') {
                ipose = (ipose + 1) % ndirs;
                where = leads_where.at(dirs[ipose]);
                next = std::make_pair(p.first + where.first, p.second + where.second);
            }

            p = next;
        } else {
            break;
        }
    }

    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            result += lines[i][j] == 'X';
        }
    }
    return result;
}

static bool hasLoop(const std::vector<std::string> &lines, pii start) {
    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());
    assert(n < NMAX && m < NMAX);
    std::unordered_set<long> used;

    const auto startPose = lines[start.first][start.second];
    auto ipose = std::distance(dirs.begin(), std::find(dirs.begin(), dirs.end(), startPose));
    auto where = leads_where.at(dirs[ipose]);

    auto p = start;
    while (true) {
        const auto state_number = p.first * m * ndirs + p.second * ndirs + ipose;
        if (used.count(state_number))
            return true;
        used.insert(state_number);
        pii next{p.first + where.first, p.second + where.second};
        if (!isGoodCoordinate(next.first, n) || !isGoodCoordinate(next.second, m)) break;

        if (lines[next.first][next.second] == '#') {
            ipose = (ipose + 1) % ndirs;
            where = leads_where.at(dirs[ipose]);
            continue;
        }

        p = next;
    }

    return false;
}

static int star2(std::vector<std::string> lines, pii start) {
    int result = 0;
    for (int i = 0; i < lines.size(); ++i) {
        for (int j = 0; j < lines[i].size(); ++j) {
            if (lines[i][j] == '.') {
                lines[i][j] = '#';

                if (hasLoop(lines, start)) {
                    result++;
                }
                lines[i][j] = '.';
            }
        }
    }
    return result;
}

int main() {
//    std::ifstream fin{"example_input.txt"};
    std::ifstream fin{"input.txt"};
    assert(fin.is_open());
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    pii start;
    for (int i = 0; i < lines.size(); ++i) {
        bool found = false;
        for (auto d : dirs) {
            if (const auto pos = lines[i].find(d); pos != std::string::npos) {
                start.first = i;
                start.second = static_cast<int>(pos);
                found = true;
                break;
            }
        }
        if (found) break;
    }

    std::cout << star2(lines, start) << std::endl;
    return 0;
}

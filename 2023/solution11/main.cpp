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
using vs = std::vector<std::string>;
using vint = std::vector<int>;

int main() {
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    vs field;

    vint emptyRows;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (std::all_of(line.begin(), line.end(), [](char c) { return c == '.'; })) {
            emptyRows.push_back(field.size());
        }
        field.push_back(line);
    }

    const int height = field.size();
    const int width = field[0].size();

    vint emptyCols;
    for (int j = 0; j < width; ++j) {
        bool allEmpty = true;
        for (int i = 0; i < height; ++i) {
            if (field[i][j] != '.') {
                allEmpty = false;
                break;
            }
        }
        if (allEmpty) {
            emptyCols.push_back(j);
        }
    }

    using Point = pii;
    std::vector<Point> galaxies;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (field[i][j] == '#') {
                galaxies.emplace_back(i, j);
            }
        }
    }

    const auto getDistance = [](int i1, int i2, const vint &emptyRows) -> int {
//        assert(i1 < i2);
        if (i1 == i2) { return 0; }
        if (i1 > i2) std::swap(i1, i2);
        auto it1 = std::lower_bound(emptyRows.begin(), emptyRows.end(), i1);
        auto it2 = std::lower_bound(emptyRows.begin(), emptyRows.end(), i2);
        return i2 - i1 + (1000000 - 1) * std::distance(it1, it2);
    };

    int64 result = 0;
    for (int i = 0; i < galaxies.size(); ++i) {
        const auto& g1 = galaxies[i];
        for (int j = 0; j < galaxies.size(); ++j) {
            const auto& g2 = galaxies[j];
            const auto rowsDistance = getDistance(g1.first, g2.first, emptyRows);
            result += rowsDistance;
            const auto colsDistance = getDistance(g1.second, g2.second, emptyCols);
            result += colsDistance;
        }
    }

    result /= 2;
    std::cout << result << std::endl;

    return 0;
}
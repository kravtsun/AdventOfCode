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

int solveRows(const vs& field)
{
    vint possibleAnswers;
    for (int i = 1; i < field.size(); ++i) {
        bool isGood = true;
        for (int iLeft = i - 1; iLeft >= 0; iLeft--) {
            const int iDiff = i - iLeft;
            int iRight = i + iDiff - 1;
            if (iRight >= field.size()) {
                continue;
            }
            if (field[iLeft] != field[iRight]) {
                isGood = false;
                break;
            }
        }
        if (isGood) {
            possibleAnswers.push_back(i);
        }
    }

    assert(possibleAnswers.size() <= 1);
    return possibleAnswers.empty() ? -1 : possibleAnswers.front();
}

int solve(vs field) {

    const int rowResult = solveRows(field);

    vs newField(field[0].size());

    for (int j = 0; j < field[0].size(); ++j) {
        for (int i = 0; i < field.size(); ++i) {
            newField[j] += field[i][j];
        }
    }

    field = newField;

    const int colResult = solveRows(field);

    assert((rowResult != -1) != (colResult != -1));
    return rowResult == -1 ? colResult : 100 * rowResult;
}

int main() {
    std::ifstream fin{WORKDIR "input_sample.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    int result = 0;
    vs field;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            result += solve(field);
            field.clear();
        } else {
            field.push_back(line);
        }
    }

    if (!field.empty()) {
        result += solve(field);
    }

    std::cout << result << std::endl;

    return 0;
}
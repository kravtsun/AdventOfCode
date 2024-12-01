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

int64 solve(const std::string &field, const vint &numbers) {
    // dp[f, i] how many ways to take f characters of `field`, and by that take i numbers
    vvint64 dp(field.size() + 1, vint64(numbers.size() + 1, 0));
    dp[0][0] = 1;

    auto tryTakeNumber = [&dp, &field, &numbers](int f) -> void {
        for (int i = 0; i < numbers.size(); ++i) {
            const auto num = numbers[i];
            if (f + num > field.size()) continue;
            bool isGood = true;
            for (int j = 0; j < num; ++j) {
                const auto nextChar = field[f + j];
                if (nextChar == '.') {
                    isGood = false;
                    break;
                }
            }
            if (f + num < field.size() && field[f + num] == '#') {
                isGood = false;
            }
            if (isGood) {
                if (f + num == field.size()) {
                    dp[field.size()][i + 1] += dp[f][i];
                } else {
                    dp[f + num + 1][i + 1] += dp[f][i];
                }
            }
        }
    };

    const auto noTakingNumbers = [&dp, &field, &numbers](int f)
    {
        for (int i = 0; i <= numbers.size(); ++i) {
            dp[f + 1][i] += dp[f][i];
        }
    };

    for (int f = 0; f < field.size(); ++f) {
        const char c = field[f];
        if (c == '.') {
            noTakingNumbers(f);
        } else if (c == '#') {
            tryTakeNumber(f);
        } else if (c == '?') {
            // no taking number: '.'
            noTakingNumbers(f);
            // taking number
            tryTakeNumber(f);
        } else {
            assert(false);
        }
    }

    return dp[field.size()][numbers.size()];
}

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    int64 result = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        std::string field;
        lineStream >> field;

        int num;
        vint numbers;
        char c;
        while (lineStream >> num) {
            numbers.push_back(num);
            lineStream >> c;
            assert(c == ',');
        }

        const int ncopies = 5;
        std::string fullField;
        vint fullNumbers;
        for (int i = 0; i < ncopies; ++i) {
            fullField += field;
            if (i + 1 != ncopies) {
                fullField += '?';
            }
            std::copy(numbers.begin(), numbers.end(), std::back_inserter(fullNumbers));
        }
        field = fullField;
        numbers = fullNumbers;

        const auto currentResult = solve(field, numbers);
        std::cout << currentResult << std::endl;
        result += currentResult;
    }

    std::cout << result << std::endl;

    return 0;
}
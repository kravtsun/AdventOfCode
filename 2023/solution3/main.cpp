#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include <cassert>
#include <cctype>
#include <cstdio>

static const std::vector<std::string> numbers = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
};

int main() {
    FILE *f = freopen(WORKDIR aoc_utils::INPUT_FILE, "r", stdin);

    std::vector<std::string> field;

    std::string line;
    while (std::getline(std::cin, line)) {
        field.push_back(line);
    }

    constexpr auto isDigit = [](char c) {
        return isdigit(c) != 0;
    };

    const int height = field.size();
    const int width = field[0].size();

    const auto gearIndex = [width](int i, int j) -> int {
        return i * width + j;
    };

    std::map<int, std::vector<int>> gearNumbers;

//    int64_t detailsResult = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (isDigit(field[i][j])) {
                int number = 0;
                int jLast = j;
                for (; jLast < width && isDigit(field[i][jLast]); ++jLast) {
                    number *= 10;
                    number += field[i][jLast] - '0';
                }
//                bool isDetailPresent = false;
                for (int ii = std::max(0, i - 1); ii <= std::min(height - 1, i + 1); ++ii) {
                    for (int jj = std::max(0, j - 1); jj <= std::min(width - 1, jLast); ++jj) {
                        char c = field[ii][jj];
//                        if (!isDigit(c) && c != '.') {
//                            isDetailPresent = true;
//                            break;
//                        }
                        if (c == '*') {
                            gearNumbers[gearIndex(ii, jj)].push_back(number);
                        }
                    }
                }
//                if (isDetailPresent) {
//                    detailsResult += number;
//                }
                j = jLast;
            }
        }
    }

//    std::cout << detailsResult << std::endl;

    int64_t result = 0;
    for (const auto& [index, numbers] : gearNumbers) {
        if (numbers.size() == 2) {
            result += numbers.back() * numbers.front();
        }
    }
    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

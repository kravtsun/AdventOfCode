#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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
    FILE *f = freopen("input.txt", "r", stdin);
    std::string line;

    struct MaxCounts {
        MaxCounts() = default;
        int r = 0, g = 0, b = 0;
    };

    // max count for r, g, b.
    std::vector<MaxCounts> games;

    while (std::getline(std::cin, line)) {
        std::istringstream is{line};
        std::string roundLine;
        is >> roundLine;
        assert(roundLine == "Game");

        int gameNumber;
        is >> gameNumber;

        char gameDelimiter;
        is >> gameDelimiter;
        assert(gameDelimiter == ':');

        MaxCounts currentMaxCounts;

        while (std::getline(is, roundLine, ';')) {
            assert(roundLine.front() == ' ');
            std::istringstream roundStream{roundLine};
            std::string setLine;
            while (std::getline(roundStream, setLine, ',')) {
                std::stringstream setStream{setLine};
                int setNumber;
                std::string setType;
                setStream >> setNumber;
                setStream >> setType;
                assert(setType == "blue" || setType == "green" || setType == "red");
                if (setType == "red") {
                    currentMaxCounts.r = std::max(currentMaxCounts.r, setNumber);
                } else if (setType == "green") {
                    currentMaxCounts.g = std::max(currentMaxCounts.g, setNumber);
                } else if (setType == "blue") {
                    currentMaxCounts.b = std::max(currentMaxCounts.b, setNumber);
                }
            }
        }
        games.push_back(currentMaxCounts);
    }

    int64_t result = 0;
    for (int i = 0; i < games.size(); ++i) {
        const auto& maxCounts = games[i];
//        if (maxCounts.r <= 12 && maxCounts.g <= 13 && maxCounts.b <= 14) {
//            result += i + 1;
//        }
        result += maxCounts.r * maxCounts.g * maxCounts.b;
    }
    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

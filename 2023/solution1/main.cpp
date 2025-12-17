#include <iostream>
#include <string>
#include <vector>
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
    FILE *f = freopen(WORKDIR "input.txt", "r", stdin);
    std::string line;

    const auto getDigitHere = [&line](int i) -> int
    {
        if (isdigit(line[i]) != 0) {
            return line[i] - '0';
        }

        for (int j = 0; j < numbers.size(); ++j) {
            const auto numString = numbers[j];

            if (line.substr(i, numString.size()) == numString) {
                return j + 1;
            }
        }
        return -1;
    };

    int result = 0;
    while (std::getline(std::cin, line)) {
        bool firstFound = false;

        int firstDigit = 0;
        int lastDigit = 0;
        for (int i = 0; i < line.size(); ++i) {
            const auto digitHere = getDigitHere(i);
            if (!firstFound && digitHere != -1) {
                firstDigit = digitHere;
                firstFound = true;
            }
            if (digitHere != -1) {
                lastDigit = digitHere;
            }
        }

        result += firstDigit * 10 + lastDigit;
    }

    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

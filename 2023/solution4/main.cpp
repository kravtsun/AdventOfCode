#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

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
    FILE *f = freopen(WORKDIR "input.txt", "r", stdin);

    std::string line;
    std::map<int, int> cardCount;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};

        std::string s;
        lineStream >> s; // Card
        int cardNumber;
        lineStream >> cardNumber;
        lineStream >> s; // :
        cardCount[cardNumber]++;
        std::string winningNumbersString;
        std::getline(lineStream, winningNumbersString, '|');
        std::istringstream winningNumbersStream{winningNumbersString};
        std::vector<int> winNumbers;
        int number;
        while (winningNumbersStream >> number) {
            winNumbers.push_back(number);
        }
        std::vector<int> ownNumbers;

        while (lineStream >> number) {
            ownNumbers.push_back(number);
        }

        int currentCardCount = cardCount.at(cardNumber);
        int nextCardNumber = cardNumber;

        for (auto num : winNumbers) {
            if (std::find(ownNumbers.begin(), ownNumbers.end(), num) != ownNumbers.end()) {
                nextCardNumber++;
                cardCount[nextCardNumber] += currentCardCount;
            }
        }
    }

    int64_t result = 0;

    for (auto [num, count] : cardCount) {
        result += count;
    }

    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

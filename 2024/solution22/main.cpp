#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>
#include <map>
#include <queue>
#include <set>

const int64_t MOD = 16777216;

static auto nextSecretNumber(int64_t num) {
    num = ((num << 6) ^ num) % MOD;
    num = (num ^ (num >> 5)) % MOD;
    num = (num ^ (num << 11)) % MOD;
    return num;
}

const int NPRICE_CHANGES = 2000;

static auto star1(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    int64_t result = 0;
    int64_t num;
    while (fin >> num) {
        std::vector<int> buyerSequence;
        for (int i = 0; i < NPRICE_CHANGES; ++i) {
            num = nextSecretNumber(num);
        }
        result += num;
    }
    return result;
}

const int SINGLE_SEQUENCE_SIZE = 4;
using SingleSequence = std::array<int, SINGLE_SEQUENCE_SIZE>;
using SingleSequenceToPrice = std::map<SingleSequence, int>;

static auto calculateSingleSequenceToPrice(int64_t num) {
    SingleSequenceToPrice singleSequenceToPrice;
    int price = static_cast<int>(num % 10);
    std::deque<int> lastSingleSequence;

    for (int i = 0; i < NPRICE_CHANGES; ++i) {
        auto nextNum = nextSecretNumber(num);
        int nextPrice = static_cast<int>(nextNum % 10);
        lastSingleSequence.push_back(nextPrice - price);
        if (lastSingleSequence.size() > SINGLE_SEQUENCE_SIZE) {
            assert(lastSingleSequence.size() == SINGLE_SEQUENCE_SIZE + 1);
            lastSingleSequence.pop_front();
        }

        if (lastSingleSequence.size() == SINGLE_SEQUENCE_SIZE) {
            SingleSequence singleSequence;
            std::copy(lastSingleSequence.begin(), lastSingleSequence.end(), singleSequence.begin());
            if (singleSequenceToPrice.count(singleSequence) == 0) {
                singleSequenceToPrice[singleSequence] = nextPrice;
            }
        }

        num = nextNum;
        price = nextPrice;
    }
    return singleSequenceToPrice;
}

static auto star2(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());

    int64_t num;

    std::vector<SingleSequenceToPrice> singleSequenceToPrices;
    std::set<SingleSequence> allSingleSequences;

    while (fin >> num) {
        auto singleSequenceToPrice = calculateSingleSequenceToPrice(num);
        singleSequenceToPrices.push_back(singleSequenceToPrice);
        for (auto [singleSequence, _]: singleSequenceToPrice) {
            allSingleSequences.insert(singleSequence);
        }
    }

    auto getPriceForSingleSequence = [&](SingleSequence singleSequence) {
        int result = 0;
        for (const auto &singleSequenceToPrice: singleSequenceToPrices) {
            auto priceForCurrentSequenceIt = singleSequenceToPrice.find(singleSequence);
            if (priceForCurrentSequenceIt != singleSequenceToPrice.end()) {
                result += priceForCurrentSequenceIt->second;
            }
        }
        return result;
    };
    int bestResult = std::numeric_limits<int>::min();

    for (auto singleSequence: allSingleSequences) {
        auto currentResult = getPriceForSingleSequence(singleSequence);
        if (currentResult > bestResult) {
            bestResult = currentResult;
        }
    }
    return bestResult;
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;
    std::cout << star2("example_input2.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;
    return 0;
}

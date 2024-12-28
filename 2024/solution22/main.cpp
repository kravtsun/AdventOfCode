#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>
#include <set>
#include <unordered_set>
#include <unordered_map>

static auto nextSecretNumber(int64_t num) {
    const int64_t MOD = 16777216;
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

static const int SINGLE_SEQUENCE_SIZE = 4;
using SingleSequence = int; // packed array<int> of size SINGLE_SEQUENCE_SIZE
using SingleSequenceToPrice = std::unordered_map<SingleSequence, int>;

static auto appendNumberToPackedSingleSequence(SingleSequence singleSequence, int priceChange) {
    const auto MINIMAL_PRICE_CHANGE = -18; // 9 - (-9)
    const auto MAXIMAL_PRICE_CHANGE = 18;  // -9 - 9
    // Pack 4-sized single sequence into an integer number.
    if (priceChange < MINIMAL_PRICE_CHANGE || priceChange > MAXIMAL_PRICE_CHANGE) {
        throw std::runtime_error("Bad difference: " + std::to_string(priceChange));
    }
    const auto PACK_MOD = 100;
    // WHOLE_PACK_MOD = PACK_MOD ^ (SINGLE_SEQUENCE_SIZE - 1) to cut off
    // a too early element for the current single sequence (by index i - SINGLE_SEQUENCE_SIZE)
    const auto WHOLE_PACK_MOD = 1'00'00'00;
    singleSequence %= WHOLE_PACK_MOD;
    singleSequence *= PACK_MOD;
    singleSequence += priceChange - MINIMAL_PRICE_CHANGE;
    return singleSequence;
}

static auto calculateSingleSequenceToPrice(const std::vector<int64_t> &nums) {
    SingleSequenceToPrice singleSequenceToPrice;
    for (auto num: nums) {
        std::unordered_set<SingleSequence> currentSingleSequences;
        int price = static_cast<int>(num % 10);
        SingleSequence singleSequence = 0;

        for (int i = 0; i < NPRICE_CHANGES; ++i) {
            auto nextNum = nextSecretNumber(num);
            int nextPrice = static_cast<int>(nextNum % 10);
            auto priceChange = nextPrice - price;
            singleSequence = appendNumberToPackedSingleSequence(singleSequence, priceChange);

            if (i + 1 >= SINGLE_SEQUENCE_SIZE) {
                auto it = currentSingleSequences.find(singleSequence);
                if (it == currentSingleSequences.end()) {
                    currentSingleSequences.insert(it, singleSequence);
                    singleSequenceToPrice[singleSequence] += nextPrice;
                }
            }

            num = nextNum;
            price = nextPrice;
        }
    }

    return singleSequenceToPrice;
}

static auto star2(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());

    int64_t num;

    std::vector<int64_t> nums;
    while (fin >> num) {
        nums.push_back(num);
    }
    auto singleSequenceToPrice = calculateSingleSequenceToPrice(nums);
    int bestPrice = std::numeric_limits<int>::min();
    for (const auto &[singleSequence, price]: singleSequenceToPrice) {
        if (price > bestPrice) {
            bestPrice = price;
        }
    }
    return bestPrice;
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl; // 37327623
    std::cout << star1("input.txt") << std::endl; // 14082561342
    std::cout << star2("example_input2.txt") << std::endl; // 23
    std::cout << star2("input.txt") << std::endl; // 1568
    return 0;
}

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>

static const int MAXPOW = 18;
static int64_t pows10[MAXPOW] = {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    10000000000,
    100000000000,
    1000000000000,
    10000000000000,
    100000000000000,
    1000000000000000,
    10000000000000000,
    100000000000000000
};

static int getNumberOfDigits(int64_t num) {
    int result = 0;
    while (num > 0) {
        result++;
        num /= 10;
    }
    return result;
}

static auto readNums(const std::string &filepath) {
    std::ifstream fin{filepath};
    std::string line;
    std::getline(fin, line);
    std::istringstream is{line};
    std::vector<int64_t> nums;
    int64_t num;
    while (is >> num) {
        nums.push_back(num);
    }
    return nums;
}

static std::vector<int64_t> blink(int64_t num) {
    std::vector<int64_t> result;
    if (num == 0) {
        result.push_back(1);
    } else if (int numDigits = getNumberOfDigits(num); numDigits % 2 == 0) {
        const auto divider = pows10[numDigits / 2];
        auto l = num / divider;
        auto r = num % divider;
        result.push_back(l);
        result.push_back(r);
    } else {
        const auto newNum = 2024 * num;
        assert(newNum >= 0);
        result.push_back(newNum);
    }
    return result;
}

static std::vector<int64_t> blink(const std::vector<int64_t> &v) {
    std::vector<int64_t> result;
    for (auto num : v) {
        auto numResult = blink(num);
        std::copy(numResult.begin(), numResult.end(), std::back_inserter(result));
    }
    return result;
}

static void printNums(const std::vector<int64_t> &nums) {
    for (auto num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

static auto star1(std::vector<int64_t> nums, size_t nblinks) {
    while (nblinks--) {
        nums = blink(nums);
    }
    return nums.size();
}

static const int MAXNBLINKS = 75 + 1;
static std::map<int64_t, size_t> memo[MAXNBLINKS];

static uint64_t star2(const std::vector<int64_t> &nums, size_t nblinks);

static uint64_t star2(int64_t num, size_t nblinks) {
    if (nblinks == 0) return 1;
    auto memoIt = memo[nblinks].find(num);
    if (memoIt != memo[nblinks].end()) {
        return memoIt->second;
    }

    auto numsAfterBlink = blink(num);
    auto result = star2(numsAfterBlink, nblinks - 1);
    memo[nblinks][num] = result;
    return result;
}

static uint64_t star2(const std::vector<int64_t> &nums, size_t nblinks) {
    uint64_t result = 0;
    for (auto num : nums) {
        result += star2(num, nblinks);
    }
    return result;
}

static auto test() {
    // filename, number of blinks -> result after specified number of blinks
    const std::map<std::pair<std::string, size_t>, size_t> expectedResults = {
            {{"example_input.txt", 1}, 7},
            {{"example_input1.txt", 6}, 22},
            {{"example_input1.txt", 25}, 55312},
    };

    for (auto [filepathNblinks, expectedSize] : expectedResults) {
        auto [filepath, nblinks] = filepathNblinks;
        auto nums = readNums(filepath);
        auto numsSize = star2(nums, nblinks);
        assert(numsSize == expectedSize);
    }
}

static auto solveStar1() {
    auto nums = readNums("input.txt");
    auto numsSize = star1(nums, 25);
    std::cout << numsSize << std::endl;
}

static auto solveStar2() {
    auto nums = readNums("input.txt");
    auto numsSize = star2(nums, 75);
    std::cout << numsSize << std::endl;
}

int main() {
    solveStar1();
    solveStar2();
    return 0;
}

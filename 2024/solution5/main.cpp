#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

// x -> set of numbers y, such as (x, y) was present in input,
// that is y (member of the set<int>) should come before x - key.
using ShouldComeBefore = std::map<int, std::set<int>>;
using PageNumbers = std::vector<int>;
using NumberUpdates = std::vector<PageNumbers>;

// returns shouldComeBefore, numberUpdates
static auto readInputs(const std::string &filename) {
    std::ifstream fin{filename};
    assert(fin.is_open());
    std::string line;

    ShouldComeBefore shouldComeBefore;
    while (std::getline(fin, line)) {
        if (line.empty()) break;
        std::istringstream is{line};
        int a, b;
        char c;
        is >> a >> c >> b;
        assert(c == '|');
        shouldComeBefore[b].insert(a);
    }

    NumberUpdates numberUpdates;
    while (std::getline(fin, line)) {
        for (auto &c: line) {
            if (c == ',') c = ' ';
        }
        std::istringstream is{line};
        int num;
        PageNumbers nums;
        while (is >> num) {
            nums.push_back(num);
        }
        numberUpdates.push_back(nums);
    }
    return std::make_tuple(shouldComeBefore, numberUpdates);
}

static bool isGoodNumbers(const ShouldComeBefore &shouldComeBefore, const PageNumbers &nums) {
    std::set<int> excludedNums;
    bool isGood = true;
    for (auto b: nums) {
        if (excludedNums.count(b)) {
            isGood = false;
            break;
        }
        if (shouldComeBefore.count(b)) {
            const auto &shouldComeBeforeThis = shouldComeBefore.at(b);
            for (auto a: shouldComeBeforeThis) {
                excludedNums.insert(a);
            }
        }
    }
    return isGood;
}

static auto star1(const std::string &filepath) {
    auto [shouldComeBefore, numberUpdates] = readInputs(filepath);
    int result = 0;
    for (auto const &nums: numberUpdates) {
        bool isGood = isGoodNumbers(shouldComeBefore, nums);
        if (isGood) {
            assert(nums.size() % 2 == 1);
            result += nums[nums.size() / 2];
        }
    }
    return result;
}

static auto star2(const std::string &filepath) {
    auto [shouldComeBefore, numberUpdates] = readInputs(filepath);
    const auto firstShouldComeBeforeSecond = [&shouldComeBefore](int first, int second) {
        auto shouldComeBeforeB = shouldComeBefore.find(second);
        return shouldComeBeforeB != shouldComeBefore.end() && shouldComeBeforeB->second.count(first);
    };
    int result = 0;
    for (auto nums: numberUpdates) {
        if (isGoodNumbers(shouldComeBefore, nums)) continue;

        std::stable_sort(nums.begin(), nums.end(), [&firstShouldComeBeforeSecond](int lhs, int rhs) {
            if (firstShouldComeBeforeSecond(lhs, rhs)) {
                return false; // rhs should come before lhs, therefore rhs < lhs
            }

            if (firstShouldComeBeforeSecond(rhs, lhs)) {
                return true; // lhs should come before rhs, therefore lhs < rhs
            }

            return false;
        });
        assert(nums.size() % 2 == 1);
        result += nums[nums.size() / 2];
    }

    return result;
}

int main() {
    std::cout << star1("example_input.txt") << std::endl; // 143
    std::cout << star1("input.txt") << std::endl; // 4185
    std::cout << star2("example_input.txt") << std::endl; // 123
    std::cout << star2("input.txt") << std::endl; // 4480
    return 0;
}

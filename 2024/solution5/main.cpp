#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

// returns shouldComeBefore, nums_of_nums
static std::tuple<std::map<int, std::set<int>>, std::vector<std::vector<int>>> inputs(const std::string &filename) {
    std::ifstream fin{filename};
    assert(fin.is_open());
    std::string line;

    std::map<int, std::set<int>> shouldComeBefore;
    while (std::getline(fin, line)) {
        if (line.empty()) break;
        std::istringstream is{line};
        int a, b;
        char c;
        is >> a >> c >> b;
        assert(c == '|');
        shouldComeBefore[b].insert(a);
    }

    std::vector<std::vector<int>> nums_of_nums;
    while (std::getline(fin, line)) {
        for (auto &c: line) {
            if (c == ',') c = ' ';
        }
        std::istringstream is{line};
        int num;
        std::vector<int> nums;
        while (is >> num) {
            nums.push_back(num);
        }
        nums_of_nums.push_back(nums);
    }
    return std::make_tuple(shouldComeBefore, nums_of_nums);
}

static bool is_good_nums(const std::map<int, std::set<int>> &shouldComeBefore, const std::vector<int> &nums) {
    std::set<int> excluded_nums;
    bool isGood = true;
    for (auto b: nums) {
        if (excluded_nums.count(b)) {
            isGood = false;
            break;
        }
        if (shouldComeBefore.count(b)) {
            const auto &shouldComeBeforeThis = shouldComeBefore.at(b);
            for (auto a: shouldComeBeforeThis) {
                excluded_nums.insert(a);
            }
        }
    }
    return isGood;
}

// 143, 4185
static int
star1(const std::map<int, std::set<int>> &shouldComeBefore, const std::vector<std::vector<int>> &nums_of_nums) {
    int result = 0;
    for (auto const &nums: nums_of_nums) {
        bool isGood = is_good_nums(shouldComeBefore, nums);
        if (isGood) {
            assert(nums.size() % 2 == 1);
            result += nums[nums.size() / 2];
        }
    }
    return result;
}

// 123, 4480
static int
star2(const std::map<int, std::set<int>> &shouldComeBefore, const std::vector<std::vector<int>> &nums_of_nums) {
    const auto a_should_be_before_b = [&shouldComeBefore](int a, int b) {
        auto shouldComeBefore_b = shouldComeBefore.find(b);
        return shouldComeBefore_b != shouldComeBefore.end() && shouldComeBefore_b->second.count(a);
    };
    int result = 0;
    for (auto nums: nums_of_nums) {
        if (is_good_nums(shouldComeBefore, nums)) continue;

        std::stable_sort(nums.begin(), nums.end(), [&a_should_be_before_b](int lhs, int rhs) {
            if (a_should_be_before_b(lhs, rhs)) {
                return false; // rhs should come before lhs, therefore rhs < lhs
            }

            if (a_should_be_before_b(rhs, lhs)) {
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
//    auto [shouldComeBefore, nums_of_nums] = inputs("example_input.txt");
    auto [shouldComeBefore, nums_of_nums] = inputs("input.txt");

    std::cout << star2(shouldComeBefore, nums_of_nums) << std::endl;
    return 0;
}

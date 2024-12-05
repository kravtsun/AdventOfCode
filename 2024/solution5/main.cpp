#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <map>
#include <set>

// returns should_not_come_after, nums_of_nums
static std::tuple<std::map<int, std::vector<int>>, std::vector<std::vector<int>>> inputs(const std::string &filename) {
    std::ifstream fin{filename};
    assert(fin.is_open());
    std::string line;

    std::map<int, std::vector<int>> should_not_come_after;
    while (std::getline(fin, line)) {
        if (line.empty()) break;
        std::istringstream is{line};
        int a, b;
        char c;
        is >> a >> c >> b;
        assert(c == '|');
        should_not_come_after[b].push_back(a);
    }

    std::vector<std::vector<int>> nums_of_nums;
    int result = 0;
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
    return std::make_tuple(should_not_come_after, nums_of_nums);
}

static bool is_good_nums(const std::map<int, std::vector<int>> &should_not_come_after, const std::vector<int> &nums) {
    std::set<int> excluded_nums;
    bool isGood = true;
    for (auto b: nums) {
        if (excluded_nums.count(b)) {
            isGood = false;
            break;
        }
        std::vector<int> should_not_come_after_this;
        if (should_not_come_after.count(b)) {
            should_not_come_after_this = should_not_come_after.at(b);
        }
        for (auto a: should_not_come_after_this) {
            excluded_nums.insert(a);
        }
    }
    return isGood;
}

// 143, 4185
static int
star1(const std::map<int, std::vector<int>> &should_not_come_after, const std::vector<std::vector<int>> &nums_of_nums) {
    int result = 0;
    for (auto const &nums: nums_of_nums) {
        bool isGood = is_good_nums(should_not_come_after, nums);
        if (isGood) {
            assert(nums.size() % 2 == 1);
            result += nums[nums.size() / 2];
        }
    }
    return result;
}

// 123, 4480
static int star2(std::map<int, std::vector<int>> should_not_come_after, std::vector<std::vector<int>> &nums_of_nums) {
    int result = 0;
    for (auto nums: nums_of_nums) {
        if (is_good_nums(should_not_come_after, nums)) continue;
        std::map<int, int> first_excluded_in_position;
        for (int i = 0; i < nums.size(); ++i) {
            if (first_excluded_in_position.count(nums[i])) {
                int pos = first_excluded_in_position.at(nums[i]);
                std::swap(nums[i], nums[pos]);
                i = -1;
                first_excluded_in_position.clear();
            } else {
                const auto &should_not_come_after_this = should_not_come_after[nums[i]];
                for (auto a: should_not_come_after_this) {
                    if (!first_excluded_in_position.count(a)) {
                        first_excluded_in_position[a] = i;
                    }
                }
            }
        }
        assert(nums.size() % 2 == 1);
        result += nums[nums.size() / 2];
    }

    return result;
}

int main() {
//    auto [should_not_come_after, nums_of_nums] = inputs("example_input.txt");
    auto [should_not_come_after, nums_of_nums] = inputs("input.txt");

    std::cout << star2(should_not_come_after, nums_of_nums) << std::endl;
    return 0;
}

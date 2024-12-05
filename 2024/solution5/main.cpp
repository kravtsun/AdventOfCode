#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <map>
#include <set>

int main() {
//    std::ifstream fin{"example_input.txt"};
    std::ifstream fin{"input.txt"};
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

        std::set<int> excluded_nums;
        bool isGood = true;
        for (auto b: nums) {
            if (excluded_nums.count(b)) {
                isGood = false;
                break;
            }
            const auto &should_not_come_after_this = should_not_come_after[b];
            for (auto a: should_not_come_after_this) {
                excluded_nums.insert(a);
            }
        }
        if (isGood) {
            assert(nums.size() % 2 == 1);
            result += nums[nums.size() / 2];
        }
    }
    std::cout << result << std::endl;
    return 0;
}

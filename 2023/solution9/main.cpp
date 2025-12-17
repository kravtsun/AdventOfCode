#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>

using vint = std::vector<int>;

static vint getDiffs(const vint& v) {
    vint diffs(v.size() - 1);
    for (int i = 0; i + 1 < v.size(); ++i) {
        diffs[i] = v[i + 1] - v[i];
    }
    return diffs;
}

static int extrapolatedLastValue(vint v) {
    assert(!v.empty());
    vint lastValues;

    while (std::count(v.begin(), v.end(), 0) != v.size()) {
        lastValues.push_back(v.back());
        v = getDiffs(v);
    }
    assert(v.back() == 0);

    int result = 0;
    for (auto it = lastValues.rbegin(); it != lastValues.rend(); ++it) {
        result += *it;
    }
    return result;
}

static int extrapolatedFirstValue(vint v) {
    assert(!v.empty());
    vint lastValues;

    while (std::count(v.begin(), v.end(), 0) != v.size()) {
        lastValues.push_back(v.front());
        v = getDiffs(v);
    }
    assert(v.front() == 0);

    int result = 0;
    for (auto it = lastValues.rbegin(); it != lastValues.rend(); ++it) {
        result = *it - result;
    }
    return result;
}

int main() {
    FILE *f = freopen(WORKDIR aoc_utils::INPUT_FILE, "r", stdin);

    int result = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        std::vector<int> numbers;
        int num;
        while (lineStream >> num) {
            numbers.push_back(num);
        }
        result += extrapolatedFirstValue(numbers);
    }

    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

// This software was partially written using Suggestions from GitHub Copilot.

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <iostream>

#include "aoc_utils/io_helpers.h"

using ID = std::uint64_t;
template<typename T>
using Range = std::pair<T, T>;
using IDRange = Range<ID>;
using IDRanges = std::vector<IDRange>;

template<typename T>
static auto mergeRanges(std::vector<Range<T>> &ranges) {
    std::vector<Range<T>> newRanges;
    if (ranges.empty()) return newRanges;

    std::sort(ranges.begin(), ranges.end());

    auto curRange = ranges.front();
    for (int i = 1; i < ranges.size(); ++i) {
        auto r = ranges[i];
        if (r.second <= curRange.second) {
            continue;
        }
        if (r.first <= curRange.second) {
            curRange.second = r.second;
        } else {
            assert(r.first > curRange.second);
            newRanges.push_back(curRange);
            curRange = r;
        }
    }
    newRanges.push_back(curRange);
    return newRanges;
}

/// @brief returns two-elements tuple of: ranges(sorted and merged) and queries
/// @param filename path to input file
static auto read_input(const std::string &filename) {
    const auto filepath = aoc_utils::get_input_file(filename, 5);
    auto lines = aoc_utils::read_lines(filepath);
    IDRanges ranges;
    int i0;
    for (i0 = 0; i0 < lines.size(); ++i0) {
        const auto line = lines[i0];
        if (line.empty()) break;
        std::istringstream is(line);
        ID a, b;
        char c;
        is >> a >> c >> b;
        assert(c == '-');
        ranges.emplace_back(a, b);
    }
    ranges = mergeRanges(ranges);

    std::vector<std::uint64_t> queries;
    for (int j = i0 + 1; j < lines.size(); ++j) {
        auto l = lines[j];
        queries.push_back(stoll(l));
    }
    return make_tuple(ranges, queries);
}

static auto star1(const std::string &filepath) {
    auto [ranges, queries] = read_input(filepath);
    int res = 0;
    for (auto num: queries) {
        bool isFresh = false;
        for (auto [a, b]: ranges) {
            if (num >= a && num <= b) {
                isFresh = true;
                break;
            }
        }
        if (isFresh) {
            res++;
        }
    }

    return res;
}

static auto star2(const std::string &filepath) {
    auto [ranges, _] = read_input(filepath);
    std::uint64_t res = 0;
    for (auto r: ranges) {
        res += r.second - r.first + 1;
    }
    return res;
}

int main() {
    std::cout << star1("example_input.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;
    std::cout << star2("example_input.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;
    return 0;
}

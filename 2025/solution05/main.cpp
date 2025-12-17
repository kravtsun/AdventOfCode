#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <format>

#include "aoc_utils/io_helpers.h"

using ID = std::uint64_t;
template<typename T>
using Range = std::pair<T, T>;
using IDRange = Range<ID>;
using IDRanges = std::vector<IDRange>;

template<typename T>
static auto merge_ranges(std::vector<Range<T>> &ranges) {
    if (ranges.empty()) return std::vector<Range<T>>{};

    std::ranges::sort(ranges);
    std::vector<Range<T>> new_ranges;

    auto cur_range = ranges.front();
    for (const auto &[start, end] : ranges | std::views::drop(1)) {
        if (end <= cur_range.second) continue;
        if (start <= cur_range.second) {
            cur_range.second = end;
        } else {
            new_ranges.push_back(cur_range);
            cur_range = {start, end};
        }
    }
    new_ranges.push_back(cur_range);
    return new_ranges;
}

/// @brief Returns a tuple of merged ranges and queries
static auto read_input(const std::string &filename) {
    const auto filepath = aoc_utils::get_input_filepath(filename, 5);
    auto lines = aoc_utils::read_lines(filepath);

    IDRanges ranges;
    auto it = std::ranges::find(lines, ""); // Find the empty line
    for (const auto &line : lines | std::views::take(it - lines.begin())) {
        std::istringstream is(line);
        ID a, b;
        char c;
        is >> a >> c >> b;
        assert(c == '-');
        ranges.emplace_back(a, b);
    }
    ranges = merge_ranges(ranges);

    std::vector<ID> queries;
    for (const auto &line : lines | std::views::drop(it - lines.begin() + 1)) {
        queries.push_back(std::stoull(line));
    }
    return std::make_tuple(ranges, queries);
}

static auto star1(const std::string &filepath) {
    const auto [ranges, queries] = read_input(filepath);
    return std::ranges::count_if(queries, [&](ID num) {
        return std::ranges::any_of(ranges, [&](const auto &[a, b]) {
            return num >= a && num <= b;
        });
    });
}

static auto star2(const std::string &filepath) {
    const auto [ranges, _] = read_input(filepath);
    return std::accumulate(ranges.begin(), ranges.end(), std::uint64_t{0}, [](auto acc, const auto &[a, b]) {
        return acc + (b - a + 1);
    });
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}
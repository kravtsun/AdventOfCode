#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <numeric>
#include <ranges>
#include <format>

#include "aoc_utils/io_helpers.h"

using int64 = std::int64_t;

// Function to get the operation (addition or multiplication) based on the sign
constexpr auto get_sign_op(char sign) {
    using op_type = std::function<int64(int64, int64)>;
    if (sign == '+') return op_type{std::plus<>()};
    if (sign == '*') return op_type{std::multiplies<>()};
    throw std::runtime_error(std::format("Unknown sign: {}", sign));
}

// Function to solve the first part of the problem
static auto star1(const std::string &filename) {
    const auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 6));
    const auto signs = lines.back() | std::views::transform([](char c) { return c; }) | std::ranges::to<std::vector>();
    const auto numbers = lines | std::views::take(lines.size() - 1)
                         | std::views::transform([](const std::string &line) {
                               std::istringstream is{line};
                               return std::vector<int64>(std::istream_iterator<int64>{is}, {});
                           })
                         | std::ranges::to<std::vector>();

    auto results = numbers.front();
    for (const auto &row : numbers | std::views::drop(1)) {
        std::ranges::transform(results, row, signs, results.begin(), [](int64 a, int64 b, char sign) {
            return get_sign_op(sign)(a, b);
        });
    }

    return std::accumulate(results.begin(), results.end(), 0LL);
}

// Function to solve the second part of the problem
static auto star2(const std::string &filename) {
    const auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 6));
    const auto signs = lines.back() | std::views::enumerate
                       | std::views::filter([](const auto &pair) { return pair.second != ' '; })
                       | std::views::transform([](const auto &pair) { return std::pair{pair.first, pair.second}; })
                       | std::ranges::to<std::vector>();
    const auto numbers = lines | std::views::take(lines.size() - 1)
                         | std::ranges::to<std::vector>();

    return std::accumulate(signs.begin(), signs.end(), 0LL, [&](int64 res, const auto &sign_pair) {
        const auto &[signPos, sign] = sign_pair;
        const auto op = get_sign_op(sign);
        const auto nextPos = (&sign_pair == &signs.back()) ? numbers[0].size() : signs[&sign_pair - &signs[0] + 1].first;

        int64 cur_res = -1;
        for (int j = nextPos - 1; j >= static_cast<int>(signPos); --j) {
            const auto cur_num = std::accumulate(numbers.begin(), numbers.end(), 0LL, [&](int64 acc, const std::string &line) {
                return line[j] != ' ' ? acc * 10 + aoc_utils::char2digit(line[j]) : acc;
            });
            if (cur_num == 0) continue;
            cur_res = (cur_res == -1) ? cur_num : op(cur_res, cur_num);
        }
        return res + cur_res;
    });
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}
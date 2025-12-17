// This software was partially written using Suggestions from GitHub Copilot.
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <numeric>

#include "aoc_utils/io_helpers.h"

using int64 = std::int64_t;

// Function to get the operation (addition or multiplication) based on the sign
static auto get_sign_op(char sign) {
    using op_type = std::function<int64(int64, int64)>;
    static const op_type add = [](int64 a, int64 b) { return a + b; };
    static const op_type multiply = [](int64 a, int64 b) { return a * b; };

    if (sign == '+') {
        return add;
    } else if (sign == '*') {
        return multiply;
    } else {
        throw std::runtime_error("Unknown sign: " + std::string(1, sign));
    }
}

// Function to solve the first part of the problem
static auto star1(const std::string &filename) {
    // Read input lines
    auto lines =
        aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 6));

    // Extract the signs from the last line
    auto signs = [](const std::string &signs_line) {
        std::istringstream is{signs_line};
        char c;
        std::vector<char> signs;
        while (is >> c) {
            signs.push_back(c);
        }
        return signs;
    }(lines.back());
    lines.pop_back();

    // Parse the numbers from the remaining lines
    std::vector<std::vector<int64>> numbers;
    numbers.reserve(lines.size());
    for (const auto &line : lines) {
        numbers.emplace_back();
        std::istringstream is{line};
        int num;
        while (is >> num) {
            numbers.back().push_back(num);
        }
    }

    // Perform calculations based on the signs
    std::vector<int64> results(signs.size());
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i].size() != signs.size()) {
            throw std::runtime_error(
                "Non-matching sizes for numbers and signs");
        }
        if (i == 0) {
            results = numbers[i];
        } else {
            for (size_t j = 0; j < signs.size(); ++j) {
                results[j] = get_sign_op(signs[j])(results[j], numbers[i][j]);
            }
        }
    }

    // Return the sum of the results
    return std::accumulate(results.begin(), results.end(), 0LL);
}

// Function to solve the second part of the problem
static auto star2(const std::string &filename) {
    // Read input lines
    auto lines =
        aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 6));

    // Extract the positions and signs from the last line
    using pic = std::pair<int, char>;
    std::vector<pic> signs;
    {
        auto signsLine = lines.back();
        for (size_t i = 0; i < signsLine.size(); ++i) {
            if (signsLine[i] != ' ') {
                signs.emplace_back(i, signsLine[i]);
            }
        }
        lines.pop_back();
    }

    int64 res = 0;

    // Perform calculations based on the signs and their positions
    for (size_t is = 0; is < signs.size(); ++is) {
        auto [signPos, sign] = signs[is];
        auto op = get_sign_op(sign);

        int nextPos =
            (is + 1 == signs.size()) ? lines[0].size() : signs[is + 1].first;
        int64 cur_res = -1;

        for (int j = nextPos - 1; j >= signPos; --j) {
            int64 cur_num = 0;
            for (const auto &line : lines) {
                if (line[j] != ' ') {
                    cur_num *= 10;
                    using aoc_utils::char2digit;
                    cur_num += char2digit(line[j]);
                }
            }
            if (cur_num == 0) continue;
            if (cur_res == -1) {
                cur_res = cur_num;
            } else {
                cur_res = op(cur_res, cur_num);
            }
        }
        res += cur_res;
    }

    return res;
}

int main() {
    // Run and print results for both parts of the problem
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}
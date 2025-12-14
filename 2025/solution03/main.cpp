// This software was partially written using Suggestions from GitHub Copilot.
#include <bits/stdc++.h>
#include "aoc_utils/io_helpers.h"

static int star1(const std::string &filepath) {
    auto lines = read_lines(filepath);

    const auto num_from_chars = [](char a, char b) {
        return char2digit(a) * 10 + char2digit(b);
    };
    int res = 0;
    for (auto line : lines) {
        auto max_it = std::max_element(line.begin(), line.end());
        auto max_char = *max_it;
        *max_it = 0;
        auto second_max_it = std::max_element(line.begin(), line.end());
        auto second_max_char = *second_max_it;
        int best_sum;

        if (max_char == second_max_char || max_it < second_max_it) {
            // The simplest option - maximum digit comes before the second
            // maximum digit.
            best_sum = num_from_chars(max_char, second_max_char);
        } else if (max_it + 1 == line.end()) {
            // Impossible to build a number with maximum digit as the first
            // digit.
            best_sum = num_from_chars(second_max_char, max_char);
        } else {
            second_max_it = std::max_element(max_it + 1, line.end());
            second_max_char = *second_max_it;
            best_sum = num_from_chars(max_char, second_max_char);
        }

        res += best_sum;
    }
    return res;
}

/**
 * @brief Run depth-first search algorithm,
 * edges are digits which we can choose, increasing current index @param i
 * @param s - digits string we traverse
 * @param i - index of current digit in string @param s
 * @param digits_rest_cnt digits left to traverse.
 * @param iteration_sum current number formed by digits we chose so far
 * @param best_sum highest possible result when running this DFS algorithm
 */
static void DFS(const std::string &s, const int i, const int digits_rest_cnt,
                const std::int64_t iteration_sum, std::int64_t &best_sum) {
    if (i == s.size() || digits_rest_cnt == 0) {
        best_sum = std::max(best_sum, iteration_sum);
        return;
    }

    // Calculate powers of 10 up to MAX_DIGITS, to save time on repeated
    // calculations.
    static const int MAX_DIGITS = 12;
    static const auto pows10 = [](const int max_digits) {
        std::vector<std::int64_t> res(max_digits + 1, 1);
        for (int i = 1; i <= max_digits; ++i) {
            res[i] = res[i - 1] * 10;
        }
        return res;
    }(MAX_DIGITS);
    if (digits_rest_cnt > MAX_DIGITS) {
        throw std::runtime_error("too big digits rest count: " +
                                 std::to_string(digits_rest_cnt));
    }

    // Best sum possible if we continue looking for the next digits from the
    // current state. In decimal form, best_sum possible is best_sum plus all 9s
    // in the remaining digits.
    const auto best_sum_possible =
        iteration_sum * pows10[digits_rest_cnt] + pows10[digits_rest_cnt] - 1;
    if (best_sum_possible < best_sum) {
        // cutoff
        return;
    }

    // used[i] holds true only if we already tried digit i as next.
    // Greedy approach - if we try taking digit i now on the first available
    // position, we do not limit our window of opportunities (so the same digit
    // can repeat)
    std::vector<bool> used(10, false);

    for (int j = i; j < s.size(); ++j) {
        char c = s[j];
        auto d = char2digit(c);
        if (!used[d]) {
            used[d] = true;
            auto cur_sum = 10 * iteration_sum + d;
            DFS(s, j + 1, digits_rest_cnt - 1, cur_sum, best_sum);
        }
    }
}

static std::int64_t star2(const std::string &filepath) {
    auto lines = read_lines(filepath);

    std::int64_t res = 0;
    for (const auto &line : lines) {
        std::int64_t best_sum = -1;
        DFS(line, 0, 12, 0, best_sum);
        res += best_sum;
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

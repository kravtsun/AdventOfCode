// This software was partially written using Suggestions from GitHub Copilot.
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "aoc_utils/io_helpers.h"
#include "aoc_utils/point.h"
#include "aoc_utils/field.h"

using int64 = std::int64_t;
using aoc_utils::Field;
using Point = aoc_utils::Field::Point;

static int64 solve(const std::vector<std::string> &lines, bool is_second_star) {
    Field field{lines};

    auto start_pos = field.symbol_location('S');

    std::queue<Point> q;
    std::map<Point, bool> used;
    std::map<Point, int64> d;
    q.push(start_pos);
    used[start_pos] = true;
    d[start_pos] = 1;

    auto try_go = [&](const Point &from, const Point &next, auto &lines) {
        if (field.is_good_point(next)) {
            d[next] += d[from];
            if (!used[next]) {
                used[next] = true;
                q.push(next);
            }
        }
    };

    int64 result = 0;

    while (!q.empty()) {
        Point p = q.front();
        q.pop();

        auto down = p.down();
        if (!field.is_good_point(down)) {
            continue;
        }

        if (field[down] == '^') {
            result += !is_second_star;
            try_go(p, down.left(), field);
            try_go(p, down.right(), field);
        } else {
            try_go(p, down, field);
        }
    }

    if (is_second_star) {
        for (int i = 0; i < field.width(); ++i) {
            result += d[{i, field.height() - 1}];
        }
    }

    return result;
}

static int64 star1(const std::string &filename) {
    auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 7));
    return solve(lines, false);
}

static int64 star2(const std::string &filename) {
    auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 7));
    return solve(lines, true);
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}
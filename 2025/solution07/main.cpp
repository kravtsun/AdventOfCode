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
    Field f{lines};

    auto start_pos = f.symbol_location('S');

    std::queue<Point> q;
    std::map<Point, bool> used;
    std::map<Point, int64> d;
    q.push(start_pos);
    used[start_pos] = true;
    d[start_pos] = 1;

    auto try_go = [&](const Point &from, const Point &next, auto &lines) {
        if (f.is_good_point(next)) {
            d[next] += d[from];
            if (!used[next]) {
                used[next] = true;
                q.push(next);
            }
        }
    };

    int64 result = 0;

    Field grid = f; // Copy grid to modify during traversal

    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        grid[p] = '|';
//        std::cout << grid << std::endl;

        auto down = p.down();
        if (!grid.is_good_point(down)) {
            continue;
        }

        if (grid[down] == '^') {
            result += !is_second_star;
            try_go(p, down.left(), grid);
            try_go(p, down.right(), grid);
        } else {
            try_go(p, down, grid);
        }
    }

    if (is_second_star) {
        for (int i = 0; i < f.width(); ++i) {
            result += d[{i, f.height() - 1}];
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
    std::cout << star1("example_input.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;
    std::cout << star2("example_input.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;
    return 0;
}
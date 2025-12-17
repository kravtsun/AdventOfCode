#include <vector>
#include <string>
#include <queue>
#include <map>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <format>
#include <numeric>

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

    auto try_go = [&](const Point &from, const Point &next) {
        if (f.is_good_point(next)) {
            d[next] += d[from];
            if (!used[next]) {
                used[next] = true;
                q.push(next);
            }
        }
    };

    int64 result = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        auto p = Point{x, y};
        auto down = p.down();

        if (!f.is_good_point(down)) continue;

        if (f[down] == '^') {
            result += !is_second_star;
            try_go(p, down.left());
            try_go(p, down.right());
        } else {
            try_go(p, down);
        }
    }

    if (is_second_star) {
        result += std::accumulate(std::views::iota(0, f.width()).begin(), std::views::iota(0, f.width()).end(), 0LL,
                                  [&](int64 acc, int i) {
                                      return acc + d[{i, f.height() - 1}];
                                  });
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
    for (const auto &file : {aoc_utils::EXAMPLE_INPUT_FILE, aoc_utils::INPUT_FILE}) {
        std::cout << std::format("Star 1 ({}): {}\n", file, star1(file));
        std::cout << std::format("Star 2 ({}): {}\n", file, star2(file));
    }
    return 0;
}

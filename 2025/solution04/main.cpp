#include "aoc_utils/io_helpers.h"
#include "aoc_utils/field.h"

#include <queue>
#include <unordered_map>
#include <ranges>
#include <iostream>
#include <algorithm>

using aoc_utils::Field;
using aoc_utils::Field;
using Point = Field::Point;

constexpr auto get_neighbor_rolls(const Field &field, const Point &p) {
    constexpr int deltas[3] = {-1, 0, 1};

    return std::views::cartesian_product(deltas, deltas)
           | std::views::filter([](const auto &delta) {
                 const auto &[dy, dx] = delta;
                 return !(dy == 0 && dx == 0);
             })
           | std::views::transform([&](const auto &delta) {
                 const auto &[dy, dx] = delta;
                 return p + Point{dx, dy};
             })
           | std::views::filter([&](const Point &pNext) {
                 return field.is_good_point(pNext) && field[pNext] == '@';
             });
}

constexpr auto star1(const std::string &filename) {
    const auto lines = read_lines(aoc_utils::get_input_filepath(filename, 4));
    Field field{lines};

    return std::ranges::count_if(std::views::iota(0, field.height())
                                     | std::views::transform([&](int i) {
                                           return std::views::iota(0, field.width())
                                                  | std::views::filter([&](int j) {
                                                        return lines[i][j] == '@';
                                                    })
                                                  | std::views::transform([&](int j) {
                                                        Point p{j, i};
                                                        return get_neighbor_rolls(field, p).size() < 4;
                                                    });
                                       }),
                                 [](bool is_accessible) { return is_accessible; });
}

constexpr auto star2(const std::string &filename) {
    const auto lines = read_lines(aoc_utils::get_input_filepath(filename, 4));
    Field field{lines};

    int res = 0;
    std::queue<Point> q;
    std::unordered_map<Point, bool> used;

    const auto try_flip_roll = [&q, &used, &field](const Point &p) {
        auto neighbor_rolls = get_neighbor_rolls(field, p);
        if (std::ranges::distance(neighbor_rolls) < 4) {
            for (const auto &pNext : neighbor_rolls) {
                if (!used[pNext]) {
                    used[pNext] = true;
                    q.push(pNext);
                }
            }
            field[p] = 'x';
            used[p] = true;
            return true;
        } else {
            used[p] = false;
            return false;
        }
    };

    for (int i = 0; i < field.height(); ++i) {
        for (int j = 0; j < field.width(); ++j) {
            if (field[{j, i}] != '@') continue;
            Point p{j, i};
            res += try_flip_roll(p);
        }
    }

    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        if (field[p] == 'x') continue;

        if (field[p] != '@') {
            throw std::runtime_error("The point in the BFS queue should be a roll");
        }

        auto neighbor_rolls = get_neighbor_rolls(field, p);
        if (std::ranges::distance(neighbor_rolls) < 4) {
            for (const auto &pNext : neighbor_rolls) {
                if (!used[pNext]) {
                    used[pNext] = true;
                    q.push(pNext);
                }
            }
            field[p] = 'x';
            used[p] = true;
            res++;
        } else {
            used[p] = false;
        }
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

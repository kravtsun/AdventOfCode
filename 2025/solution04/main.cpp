#include "aoc_utils/io_helpers.h"
#include "aoc_utils/field.h"

#include <queue>
#include <map>
#include <ranges>
#include <iostream>

using aoc_utils::Field;
using aoc_utils::read_lines;
using Point = Field::Point;

static auto get_neighbor_rolls(const Field &field, const Point &p) {
    constexpr int deltas[3] = {-1, 0, 1};

    std::vector<Point> neighbor_rolls{};
    for (int dx : deltas) {
        for (int dy : deltas) {
            if (dx == 0 && dy == 0) continue;
            auto p_next = p + Point{dx, dy};
            if (field.is_good_point(p_next) && field[p_next] == '@') {
                neighbor_rolls.push_back(p_next);
            }
        }
    }
    return neighbor_rolls;
}

static auto star1(const std::string &filename) {
    const auto lines = read_lines(aoc_utils::get_input_filepath(filename, 4));
    Field field{lines};

    int res = 0;
    for (int i = 0; i < field.height(); ++i) {
        for (int j = 0; j < field.width(); ++j) {
            Point p{j, i};
            if (field[p] != '@') {
                // Continue if we do not stand now upon a roll of paper
                continue;
            }

            auto cnt = get_neighbor_rolls(field, p).size();
            if (cnt < 4) {
                // check if a roll is accessible
                res++;
            }
        }
    }
    return res;
}

static auto star2(const std::string &filename) {
    const auto lines = read_lines(aoc_utils::get_input_filepath(filename, 4));
    Field field{lines};

    int res = 0;
    std::queue<Point> q;
    std::map<Point, bool> used;

    const auto try_flip_roll = [&q, &used, &field](const Point &p) {
        auto neighbor_rolls = get_neighbor_rolls(field, p);
        if (std::ranges::distance(neighbor_rolls) < 4) {
            for (const auto &p_next : neighbor_rolls) {
                if (!used[p_next]) {
                    used[p_next] = true;
                    q.push(p_next);
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
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}

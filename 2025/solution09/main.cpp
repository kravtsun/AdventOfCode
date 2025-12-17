#include <vector>
#include <string>
#include <iostream>
#include <ranges>
#include <format>
#include <algorithm>
#include <cmath>
#include "aoc_utils/io_helpers.h"
#include "aoc_utils/point.h"

using Point = aoc_utils::PointTmpl<std::int64_t, 2>;
using PointDouble = aoc_utils::PointTmpl<double, 2>;
using PointInt = aoc_utils::PointTmpl<std::int64_t, 2>;

static auto read_points(const std::string &filename) {
    auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 9));
    std::vector<Point> points;
    for (const auto &line : lines) {
        std::istringstream is(line);
        int a, b;
        char c;
        is >> a >> c >> b;
        points.emplace_back(a, b);
    }
    return points;
}

static auto star1(const std::string &filename) {
    auto points = read_points(filename);
    return std::ranges::max(points | std::views::transform([&](const auto &p1) {
                                return std::ranges::max(points | std::views::transform([&](const auto &p2) {
                                                            return std::abs(p1.x() - p2.x() + 1) * std::abs(p1.y() - p2.y() + 1);
                                                        }));
                            }));
}

static auto star2(const std::string &filename) {
    auto points = read_points(filename);
    const auto n = points.size();

    std::vector<PointDouble> points_extended;
    for (std::size_t i = 0; i < n; ++i) {
        points_extended.emplace_back(points[i].x(), points[i].y());
        auto i_next = (i + 1) % n;
        points_extended.emplace_back(points[i_next].x(), points[i_next].y());
        points_extended.push_back((points[i] + points[i_next]) * 0.5);
    }

    return std::ranges::max(points | std::views::transform([&](const auto &p1) {
                                return std::ranges::max(points | std::views::transform([&](const auto &p2) {
                                                            auto cur_area = std::abs(p1.x() - p2.x() + 1) * std::abs(p1.y() - p2.y() + 1);
                                                            PointInt p_min{std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y())};
                                                            PointInt p_max{std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y())};
                                                            return std::ranges::all_of(points_extended, [&](const auto &p) {
                                                                return !(p.x() > p_min.x() && p.x() < p_max.x() && p.y() > p_min.y() && p.y() < p_max.y());
                                                            }) ? cur_area : 0;
                                                        }));
                            }));
}

int main() {
    for (const auto &file : {aoc_utils::EXAMPLE_INPUT_FILE, aoc_utils::INPUT_FILE}) {
        std::cout << std::format("Star 1 ({}): {}\n", file, star1(file));
        std::cout << std::format("Star 2 ({}): {}\n", file, star2(file));
    }
    return 0;
}
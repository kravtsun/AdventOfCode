// This software was partially written using Suggestions from GitHub Copilot.
#include <bits/stdc++.h>

#include <utility>
#include "aoc_utils/io_helpers.h"
#include "aoc_utils/point.h"

using namespace std;

static auto star1(const std::string &filename) {
    using Point = aoc_utils::PointTmpl<std::int64_t, 2>;
    auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 9));

    vector<Point> points;
    for (const auto& line : lines) {
        istringstream is(line);
        int a, b;
        char c;
        is >> a >> c >> b;
        points.emplace_back(a, b);
    }

    std::int64_t res = 0;
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            auto cur_area = std::abs(points[i].x() - points[j].x() + 1) * std::abs(points[i].y() - points[j].y() + 1);
            res = std::max(res, cur_area);
        }
    }

    return res;
}

static auto star2(const std::string &filename) {
    auto lines = aoc_utils::read_lines(aoc_utils::get_input_filepath(filename, 9));
    using PointDouble = aoc_utils::PointTmpl<double, 2>;
    using PointInt = aoc_utils::PointTmpl<int64_t, 2>;
    vector<PointInt> points;
    for (const auto& line : lines) {
        istringstream is(line);
        int a, b;
        char c;
        is >> a >> c >> b;
        points.emplace_back(a, b);
    }
    const auto n = points.size();

    // It is OK to have duplicates here
    vector<PointDouble> points_extended;
    for (int i = 0; i < n; ++i) {
        points_extended.emplace_back(points[i].x(), points[i].y());
        auto i_next = (i + 1) % n;
        points_extended.emplace_back(points[i_next].x(), points[i_next].y());
        points_extended.push_back((points[i] + points[i_next]) * 0.5);
    }

    std::int64_t res = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            bool isGood = true;
            auto curArea = std::abs(points[i].x() - points[j].x() + 1) * std::abs(points[i].y() - points[j].y() + 1);
            PointInt p_min{std::min(points[i].x(), points[j].x()), std::min(points[i].y(), points[j].y())};
            PointInt p_max{std::max(points[i].x(), points[j].x()), std::max(points[i].y(), points[j].y())};
            for (auto p : points_extended) {
                if (p.x() > p_min.x() && p.x() < p_max.x() &&
                    p.y() > p_min.y() && p.y() < p_max.y()) {
                    isGood = false;
                    break;
                }
            }

            if (isGood) {
                res = std::max(res, curArea);
            }
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

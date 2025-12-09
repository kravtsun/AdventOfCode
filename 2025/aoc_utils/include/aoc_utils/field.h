#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "aoc_utils/point.h"

namespace aoc_utils {

class Field {
   public:
    using Point = aoc_utils::PointTmpl<int, 2>;
    explicit Field(std::vector<std::string> lines);

    [[nodiscard]] bool is_good_point(const Point &p) const;

    char &operator[](const Point &p);

    const char &operator[](const Point &p) const;

    [[nodiscard]] int height() const;

    [[nodiscard]] int width() const;

    [[nodiscard]] const std::vector<std::string> &get_lines() const;;

    [[nodiscard]] Point symbol_location(char symbol) const;

   private:
    std::vector<std::string> lines;
    void checkLines() const;
};

std::ostream &operator<<(std::ostream & os, const Field &f);

} // namespace aoc_utils
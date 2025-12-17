#include "aoc_utils/field.h"
#include "aoc_utils/io_helpers.h"
#include <cassert>

namespace aoc_utils {

Field::Field(std::vector<std::string> lines) : lines(std::move(lines)) {
    checkLines();
}

bool Field::is_good_point(const Field::Point &p) const {
    const auto n = height();
    const auto m = width();
    return p.y() >= 0 && p.y() < n && p.x() >= 0 && p.x() < m;
}

char &Field::operator[](const Field::Point &p) {
    if (!is_good_point(p)) {
        throw std::runtime_error("Bad point: " + p.to_string());
    }
    return lines[p.y()][p.x()];
}

const char &Field::operator[](const Field::Point &p) const {
    return lines[p.y()][p.x()];
}

int Field::height() const {
    return lines.size();
}

int Field::width() const {
    return lines[0].size();
}

const std::vector<std::string> &Field::get_lines() const {
    return lines;
}

Field::Point Field::symbol_location(char symbol) const {
    for (int i = 0; i < height(); ++i) {
        for (int j = 0; j < width(); ++j) {
            if (lines[i][j] == symbol) {
                return Point{j, i};
            }
        }
    }
    throw std::runtime_error("Not found symbol: " + std::string(1, symbol));
}

void Field::checkLines() const {
    assert(!lines.empty());
    auto w = lines[0].size();
    for (const auto &l : lines) {
        if (l.size() != w) {
            throw std::runtime_error("Inconsistent line: \n" + l);
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Field &f) {
    for (const auto& line : f.get_lines()) {
        os << line << std::endl;
    }
    return os;
}

}  // namespace aoc_utils

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include <fstream>

#include <cassert>
#include <cctype>

// REVIEW: naming like this is highly unrecommended,
// but since the code is very concentrated it is OK to name types like this here.
using int64 = uint64_t; // REVIEW: can be misleading, like #define true false
using vint64 = std::vector<int64>;

static vint64 readSeedNumbers() {
    std::string headerLine, dummyLine;
    std::getline(std::cin, headerLine);
    std::getline(std::cin, dummyLine); // empty line

    std::istringstream headerStream{headerLine};
    headerStream >> dummyLine; // seeds:
    int64 seedNumber;
    vint64 seedNumbers;
    while (headerStream >> seedNumber) {
        seedNumbers.push_back(seedNumber);
    }
    return seedNumbers;
}

// REVIEW: inclusiveness of the Range is a range's property.
struct Range {
    int64 src; // REVIEW: non-descriptive.
    int64 dst; // REVIEW: non-descriptive.
    int64 length;

    Range() = default;
};

struct SectionMap {
    std::vector<Range> ranges;

    // REVIEW: [[nodiscard]] - decreases readability?
    [[nodiscard]] int64 getMapping(int64 src) const {
        for (const auto &r: ranges) {
            if (r.src <= src && src < r.src + r.length) {
                return r.dst + src - r.src;
            }
        }
        return src;
    }

    [[nodiscard]] bool empty() const {
        return ranges.empty();
    }
};

static SectionMap readSection() {
    std::string headerLine;
    std::getline(std::cin, headerLine);
    if (headerLine.empty())
        return {};

    SectionMap result;
    for (std::string line; std::getline(std::cin, line) && !line.empty();) {
        std::istringstream lineStream{line};
        Range range{};
        lineStream >> range.dst >> range.src >> range.length;
        result.ranges.push_back(range);
    }
    return result;
}


int main() {
    // REVIEW: for freopen f is not used anywhere, can be misleading.
    // REVIEW: will be good to check if file is really open.
    std::ifstream fin{std::string(WORKDIR) + "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    auto numbers = readSeedNumbers();

    while (true) {
        const auto sectionMap = readSection();
        if (sectionMap.empty()) break;
        vint64 newNumbers;
        newNumbers.reserve(numbers.size());
        std::transform(numbers.begin(), numbers.end(), std::back_inserter(newNumbers),
                       [&sectionMap](int64 num) {
                           return sectionMap.getMapping(num);
                       });
        numbers = newNumbers;
    }

    const auto result = *std::min_element(numbers.begin(), numbers.end());
    std::cout << result << std::endl;

    return 0;
}

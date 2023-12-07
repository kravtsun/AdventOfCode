#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>

using vint = std::vector<int>;
using int64 = uint64_t;
using vint64 = std::vector<int64>;

static vint64 readSeedNumbers()
{
    std::string headerLine, line;
    std::getline(std::cin, headerLine);
    std::getline(std::cin, line); // empty line

    std::istringstream headerStream{headerLine};
    headerStream >> line; // seeds:
    int64 seedNumber;
    vint64 seedNumbers;
    while (headerStream >> seedNumber) {
        seedNumbers.push_back(seedNumber);
    }
    return seedNumbers;
}

struct Range {
    int64 src;
    int64 dst;
    int64 length;
    Range() = default;
};

struct SectionMap {
    std::vector<Range> ranges;

    [[nodiscard]] int64 getMapping(int64 src) const
    {
        for (const auto& r : ranges) {
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

static SectionMap readSection()
{
    std::string headerLine;
    std::getline(std::cin, headerLine);
    if (headerLine.empty())
        return {};

    SectionMap result;
    for (std::string line; std::getline(std::cin, line) && !line.empty(); )
    {
        std::istringstream lineStream{line};
        Range range{};
        lineStream >> range.dst >> range.src >> range.length;
        result.ranges.push_back(range);
    }
    return result;
}



int main() {
    FILE *f = freopen("input.txt", "r", stdin);

    auto numbers = readSeedNumbers();

    while (true) {
        const auto sectionMap = readSection();
        if (sectionMap.empty()) break;
        vint64 newNumbers;
        newNumbers.reserve(numbers.size());
        for (auto num : numbers) {
            newNumbers.push_back(sectionMap.getMapping(num));
        }
        numbers = newNumbers;
    }

    const auto result = *std::min_element(numbers.begin(), numbers.end());
    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

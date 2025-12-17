#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <set>
#include <queue>
#include <fstream>


using int64 = uint64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

void printField(const vs& field) {
    for (const auto&f : field) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
}

int hashString(const std::string& s)
{
    int result = 0;
    for (auto c : s) {
        result += static_cast<int>(c);
        result *= 17;
        result %= 256;
    }
    return result;
}

int main() {
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::string line;

    using psi = std::pair<std::string, int>;
    std::map<int, std::vector<psi>> maps;

    while (std::getline(std::cin, line, ',')) {
        const auto equalPos = line.find('=');
        char sign;
        if (equalPos != std::string::npos) {
            line[equalPos] = ' ';
            sign = '=';
        } else {
            const auto minusPos = line.find('-');
            assert(minusPos != std::string::npos);
            line[minusPos] = ' ';
            sign = '-';
        }

        std::istringstream lineStream{line};
        std::string label;
        int focalLength;
        lineStream >> label >> focalLength;

        const auto boxNumber = hashString(label);
        auto& box = maps[boxNumber];

        const auto findByLabel = [&box](const std::string& label) {
            return std::find_if(box.begin(), box.end(), [&label](const psi& p) {
                return p.first == label;
            });
        };

        auto labelEntry = findByLabel(label);
        if (sign == '=') {
            if (labelEntry != box.end()) {
                labelEntry->second = focalLength;
            } else {
                box.emplace_back(label, focalLength);
            }
        } else {
            if (labelEntry != box.end()) {
                box.erase(labelEntry);
            }
        }
    }

    int64 result = 0;
    for (const auto& [ibox, box] : maps) {
        for (int islot = 0; islot < box.size(); ++islot) {
            const auto [label, focalLength] = box[islot];
            result += (ibox + 1) * (islot + 1) * focalLength;
        }
    }

    std::cout << result << std::endl;

    return 0;
}
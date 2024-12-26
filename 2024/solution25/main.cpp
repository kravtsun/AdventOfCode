#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <array>
#include <algorithm>

static auto star1(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());

    constexpr auto NPATTERNS = 5;
    using Pattern = std::array<int, NPATTERNS>;
    std::vector<Pattern> keys;
    std::vector<Pattern> locks;

    while (true) {
        std::vector<std::string> lines;
        std::string line;
        bool emptyLine = false;
        while (std::getline(fin, line)) {
            if (line.empty()) {
                emptyLine = true;
                break;
            }
            lines.push_back(line);
        }

        assert(lines[0].size() == NPATTERNS);
        assert(lines.size() == 1 + NPATTERNS + 1);

        auto onlyWithSymbol = [](const std::string &s, char symbol) {
            return s == std::string(s.size(), symbol);
        };

        bool isLock = onlyWithSymbol(lines[0], '#') && onlyWithSymbol(lines.back(), '.');
        bool isKey = onlyWithSymbol(lines[0], '.') && onlyWithSymbol(lines.back(), '#');
        if (isLock) {
            Pattern pattern{};
            for (int i = 1; i < lines.size(); ++i) {
                for (int j = 0; j < lines[0].size(); ++j) {
                    if (lines[i][j] == '#') {
                        pattern[j] = i;
                    }
                }
            }
            locks.push_back(pattern);
        } else if (isKey) {
            Pattern pattern{};
            for (int i = static_cast<int>(lines.size()) - 1; i >= 0; --i) {
                for (int j = 0; j < lines[0].size(); ++j) {
                    if (lines[i][j] == '#') {
                        pattern[j] = static_cast<int>(lines.size()) - 1 - i;
                    }
                }
            }
            keys.push_back(pattern);
        } else {
            assert(false);
        }

        if (!emptyLine) {
            break;
        }
    }

    int result = 0;
    for (auto key: keys) {
        for (auto lock: locks) {
            Pattern keyLockSum;
            std::transform(key.begin(), key.end(), lock.begin(), keyLockSum.begin(), [](int a, int b) {
                return a + b;
            });
            if (std::all_of(keyLockSum.begin(), keyLockSum.end(), [](int num) {
                return num <= 5;
            })) {
                result++;
            }
        }
    }
    return result;
}

int main() {
    std::cout << star1("example_input.txt") << std::endl; // 3
    std::cout << star1("input.txt") << std::endl; // 3671
    return 0;
}

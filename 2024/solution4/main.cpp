#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <queue>
#include <set>
#include <map>
#include <functional>

static int star1(const std::vector<std::string> &lines) {
    const std::string sample = "XMAS";
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    const auto sampleSize = static_cast<int>(sample.size());

    int result = 0;
    for (int i = 0; i < n; ++i) {
        assert(lines[i].size() == m);
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] != sample.front())
                continue;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    const auto lastI = i + (sampleSize - 1) * di;
                    const auto lastJ = j + (sampleSize - 1) * dj;
                    if (lastI < 0 || lastI >= n) continue;
                    if (lastJ < 0 || lastJ >= m) continue;
                    bool isGood = true;
                    for (int k = 1; k < sampleSize; ++k) {
                        const auto nextI = i + k * di;
                        const auto nextJ = j + k * dj;
                        if (lines[nextI][nextJ] != sample[k]) {
                            isGood = false;
                            break;
                        }
                    }
                    if (isGood) result++;
                }
            }
        }
    }
    return result;
}

int star2(const std::vector<std::string> &lines) {
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    const auto isPairGood = [](char l, char r) {
        return l == 'M' && r == 'S' ||
                l == 'S' && r == 'M';
    };
    int result = 0;
    for (int i = 1; i + 1 < n; ++i) {
        assert(lines[i].size() == m);
        for (int j = 1; j + 1 < m; ++j) {
            if (lines[i][j] != 'A') continue;
            std::array lefts{lines[i - 1][j - 1], lines[i + 1][j - 1]};
            std::array rights{lines[i - 1][j + 1], lines[i + 1][j + 1]};
            if (isPairGood(lefts[0], rights[1]) && isPairGood(lefts[1], rights[0])) {
                result++;
            }
        }
    }
    return result;
}

int main() {
//    std::ifstream fin{"example_input.txt"};
    std::ifstream fin{"input.txt"};
    assert(fin.is_open());

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }

    std::cout << star2(lines) << std::endl;

    return 0;
}

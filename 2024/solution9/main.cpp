#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <set>
#include <map>
#include <queue>

struct NumberCount {
    // for free blocks number is -1
    size_t number;
    size_t count;

    NumberCount(size_t number, size_t count) : number(number), count(count) {}
};

std::vector<NumberCount> readFiles(const std::string &line) {
    std::vector<NumberCount> mas;
    bool isFree = false;
    for (int i = 0, index = 0; i < line.size(); ++i) {
        auto c = line[i];
        assert(isdigit(c));
        const auto cnt = static_cast<size_t>(c - '0');
        if (isFree) {
            mas.emplace_back(-1, cnt);
        } else {
            mas.emplace_back(index++, cnt);
        }
        isFree = !isFree;
    }
    return mas;
}

uint64_t hashsum(const std::vector<NumberCount> &resultMas) {
    size_t index = 0;
    uint64_t resultSum = 0;
    for (auto [num, cnt]: resultMas) {
        if (num == -1) {
            index += cnt;
            continue;
        }

        resultSum += num * (index * cnt + cnt * (cnt - 1) / 2);
        index += cnt;
    }
    return resultSum;
}

static auto readLine(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;
    std::getline(fin, line);
    return line;
}

static auto star1(const std::string &filepath) {
    auto line = readLine(filepath);
    auto mas = readFiles(line);
    std::vector<NumberCount> resultMas;
    auto rit = mas.rbegin();
    if (rit->number == -1) ++rit;

    for (auto [num, cnt]: mas) {
        if (num != -1) {
            resultMas.emplace_back(num, cnt);
            if (rit != mas.rend() && rit->number <= num) break;
            continue;
        }

        while (cnt > 0 && rit != mas.rend()) {
            assert(rit->number != -1);
            auto subtract_cnt = std::min(cnt, rit->count);
            cnt -= subtract_cnt;
            rit->count -= subtract_cnt;
            resultMas.emplace_back(rit->number, subtract_cnt);
            if (rit->count == 0) {
                ++rit;
                assert(rit != mas.rend());
                ++rit;
            }
        }
    }

    return hashsum(resultMas);
}

static auto star2(const std::string &filepath) {
    auto line = readLine(filepath);
    auto mas = readFiles(line);

    // blockCount -> indexes
    std::map<size_t, std::priority_queue<size_t, std::vector<size_t>, std::greater<>>> spacesByBlockCount;

    std::vector<NumberCount> blocks;
    std::vector<size_t> blockIndex;
    size_t index = 0;
    for (auto [num, cnt]: mas) {
        if (num != -1) {
            blocks.emplace_back(num, cnt);
            blockIndex.push_back(index);
        } else {
            spacesByBlockCount[cnt].push(index);
        }
        index += cnt;
    }

    for (auto ib = static_cast<int>(blocks.size()) - 1; ib >= 0; --ib) {
        auto [num, cnt] = blocks[ib];

        auto spaceIt = spacesByBlockCount.lower_bound(cnt);
        if (spaceIt == spacesByBlockCount.end()) continue;

        auto bestSpaceIt = spaceIt;
        for (auto it = std::next(spaceIt); it != spacesByBlockCount.end(); ++it) {
            assert(!it->second.empty());
            if (it->second.top() < bestSpaceIt->second.top()) {
                bestSpaceIt = it;
            }
        }

        auto bestSpaceCount = bestSpaceIt->first;
        auto bestSpaceIndex = bestSpaceIt->second.top();

        if (bestSpaceIndex > blockIndex[ib]) continue;
        blockIndex[ib] = bestSpaceIndex;

        bestSpaceIt->second.pop();
        if (bestSpaceIt->second.empty()) {
            spacesByBlockCount.erase(bestSpaceIt);
        }

        if (bestSpaceCount > cnt) {
            auto newSpaceCount = bestSpaceCount - cnt;
            auto newSpaceIndex = bestSpaceIndex + cnt;
            spacesByBlockCount[newSpaceCount].push(newSpaceIndex);
        }
    }

    uint64_t result = 0;
    for (int ib = 0; ib < blocks.size(); ++ib) {
        auto [num, cnt] = blocks[ib];
        result += num * (2 * blockIndex[ib] + cnt - 1) * cnt / 2;
    }

    return result;
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 1928
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl; // 6346871685398
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 2858
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl; // 6373055193464
    return 0;
}

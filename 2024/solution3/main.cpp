#include <iostream>
#include <cassert>
#include <fstream>

static int parseNumber(const std::string &line, size_t &offset) {
    if (offset == line.size()) {
        return -1;
    }

    if (!isdigit(line[offset])) {
        return -2;
    }

    auto start_offset = offset;
    int result = 0;
    while (offset < line.size()) {
        if (isdigit(line[offset])) {
            result *= 10;
            result += static_cast<int>(line[offset] - '0');
        } else {
            break;
        }
        if (result > 1000) {
            offset = start_offset;
            return -3;
        }
        offset++;
    }
    return result;
}

static int mulResult(const std::string &line, size_t &offset) {
    if (line.substr(offset, 3) != "mul") return -1;
    offset += 3;

    if (line[offset] != '(') return -2;
    offset++;

    auto a = parseNumber(line, offset);
    if (a < 0) return -3;

    if (line[offset] != ',') return -4;
    offset++;

    auto b = parseNumber(line, offset);
    if (b < 0) return -5;

    if (line[offset] != ')') return -6;

    return a * b;
}

static int64_t processProgram(const std::string &line, bool isFirstStar) {
    const std::string disabler = "don't()";
    const std::string enabler = "do()";
    int64_t result = 0;
    bool isEnabled = true;

    auto checkAndSetToggle = [&](size_t &offset, const std::string &toggleString) {
        if (line.substr(offset, toggleString.size()) == toggleString) {
            if (toggleString == enabler) {
                isEnabled = true;
            } else if (toggleString == disabler) {
                isEnabled = false;
            } else {
                assert(false);
            }
            offset += toggleString.size() - 1;
            return true;
        }
        return false;
    };

    for (size_t offset = 0; offset < line.size(); ++offset) {
        if (!isFirstStar && (checkAndSetToggle(offset, enabler) || checkAndSetToggle(offset, disabler))) continue;
        if (!isEnabled) continue;
        if (auto current = mulResult(line, offset); current > 0) {
            result += current;
        }
    }
    return result;
}

static auto inputProgram(const std::string &filepath) {
    std::ifstream fin{filepath};
    std::string program;
    std::string line;
    while (std::getline(fin, line)) {
        program += line;
    }
    return program;
}

static auto star1(const std::string &filepath) {
    auto program = inputProgram(filepath);
    return processProgram(program, true);
}

static auto star2(const std::string &filepath) {
    auto program = inputProgram(filepath);
    return processProgram(program, false);
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 161
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl; // 160672468
    std::cout << star2("example_input2.txt") << std::endl; // 48
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl; // 84893551
    return 0;
}

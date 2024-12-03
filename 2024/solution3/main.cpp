#include <iostream>
#include <cassert>
#include <fstream>

int parse_number(const std::string &line, size_t &offset) {
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

int mul_result(const std::string &line, size_t offset) {
    if (line.substr(offset, 3) != "mul") return -1;
    offset += 3;

    if (line[offset] != '(') return -2;
    offset++;

    auto a = parse_number(line, offset);
    if (a < 0) return -3;

    if (line[offset] != ',') return -4;
    offset++;

    auto b = parse_number(line, offset);
    if (b < 0) return -5;

    if (line[offset] != ')') return -6;

    return a * b;
}

int64_t process_program(const std::string &line, bool isFirstStar = false) {
    const std::string disabler = "don't()";
    const std::string enabler = "do()";
    int64_t result = 0;
    bool isEnabled = true;

    auto checkToggle = [&](size_t &offset, const std::string &toggleString) {
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
        if (!isFirstStar && (checkToggle(offset, enabler) || checkToggle(offset, disabler))) {
            continue;
        }
        if (!isEnabled) continue;
        if (auto current = mul_result(line, offset); current > 0) {
            result += current;
        }
    }
    return result;
}

int main() {
//    std::ifstream fin{"example_input2.txt"};
    std::ifstream fin{"input.txt"};

    std::string program;
    std::string line;
    while (std::getline(fin, line)) {
        program += line;
    }
    std::cout << process_program(program) << std::endl;

    return 0;
}

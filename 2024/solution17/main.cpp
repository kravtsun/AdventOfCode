#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>
#include <array>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <map>

const int NREGISTERS = 3;
using OperandType = int64_t;
using Registers = std::array<OperandType, NREGISTERS>;

template<typename Token>
static void readToken(std::istream &is, const std::vector<Token> &expectedTokens) {
    Token token;
    is >> token;
    auto it = std::find(expectedTokens.begin(), expectedTokens.end(), token);
    if (it == expectedTokens.end()) {
        std::ostringstream os;
        os << "Read: " << token << ", Expected: ";
        for (const auto &t: expectedTokens) {
            os << t << ",";
        }
        throw std::runtime_error(os.str());
    }
}

static auto readData(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;

    Registers registers;
    for (int i = 0; i < NREGISTERS; ++i) {
        std::getline(fin, line);
        std::istringstream is{line};
        readToken<std::string>(is, {"Register"});
        std::string registerString = "A:";
        registerString[0] += i;
        readToken<std::string>(is, {registerString});
        is >> registers[i];
    }
    std::getline(fin, line);
    assert(line.empty());
    readToken<std::string>(fin, {"Program:"});
    std::string program;
    fin >> program;
    assert(!program.empty() && program.front() != ' ');

    return std::make_tuple(registers, program);
}

static auto programToNumbers(const std::string &program) {
    std::vector<OperandType> result;
    for (int i = 0; i < program.size(); ++i) {
        if (i % 2 == 1 && program[i] == ',') {
            continue;
        }

        if (program[i] == ',') {
            throw std::runtime_error("Unexpected comma: " + std::to_string(i));
        }

        auto token = program[i];
        result.push_back(token - '0');
    }
    return result;
}

static auto literal(OperandType num) {
    if (num < 0 || num >= 7) {
        throw std::runtime_error("Should be a digit: " + std::to_string(num));
    }
    return num;
}

static auto combo(Registers &registers, OperandType num) {
    if (num < 0 || num >= 7) {
        throw std::runtime_error("Bad digit: " + std::to_string(num));
    }

    if (num <= 3) {
        return num;
    }
    return registers[num - 4];
}

static auto adv(Registers &registers, OperandType operandToken) {
    // The numerator is the value in the A register.
    // The denominator is found by raising 2 to the power of the instruction's combo operand.
    // (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.)
    // The result of the division operation is truncated to an integer and then written to the A register.
    return registers[0] >> combo(registers, operandToken);
}

static auto runProgram(Registers registers, const std::vector<OperandType> &programNumbers) {
    std::vector<OperandType> result;
    for (int i = 0; i < programNumbers.size();) {
        auto instructionToken = programNumbers[i];
        auto operandToken = programNumbers[i + 1];
        i += 2;

        switch (instructionToken) {
            case 0:
                // The adv instruction (opcode 0) performs division.
                registers[0] = adv(registers, operandToken);
                break;
            case 1:
                // The bxl instruction (opcode 1) calculates the bitwise XOR of register B and
                // the instruction's literal operand, then stores the result in register B.
                registers[1] = registers[1] ^ literal(operandToken);
                break;
            case 2:
                // The bst instruction (opcode 2) calculates the value of its combo operand modulo 8
                // (thereby keeping only its lowest 3 bits), then writes that value to the B register.
                registers[1] = combo(registers, operandToken) % 8;
                break;
            case 3:
                // The jnz instruction (opcode 3) does nothing if the A register is 0.
                // However, if the A register is not zero, it jumps by setting the instruction pointer to the value of its literal operand;
                // if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
                if (registers[0] != 0) {
                    i = 2 * literal(operandToken);
                }
                break;
            case 4:
                // The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C,
                // then stores the result in register B.
                // (For legacy reasons, this instruction reads an operand but ignores it.)
                registers[1] = registers[1] ^ registers[2];
                break;
            case 5:
                // The out instruction (opcode 5) calculates the value of its combo operand modulo 8,
                // then outputs that value. (If a program outputs multiple values, they are separated by commas.)
                result.push_back(combo(registers, operandToken) % 8);
                break;
            case 6:
                // The bdv instruction (opcode 6) works exactly like the adv instruction
                // except that the result is stored in the B register.
                // (The numerator is still read from the A register.)
                registers[1] = adv(registers, operandToken);
                break;
            case 7:
                // The cdv instruction (opcode 7) works exactly like the adv instruction
                // except that the result is stored in the C register.
                // (The numerator is still read from the A register.)
                registers[2] = adv(registers, operandToken);
                break;
            default:
                throw std::runtime_error("Bad instruction: " + std::to_string(instructionToken));
        }
    }
    return result;
}

static auto star1(const std::string &filepath) {
    auto [registers, program] = readData(filepath);
    const auto programNumbers = programToNumbers(program);
    auto result = runProgram(registers, programNumbers);
    std::ostringstream os;
    for (int i = 0; i < result.size(); ++i) {
        if (i > 0) {
            os << ",";
        }
        os << result[i];
    }
    return os.str();
}

static auto star2BruteForce(const std::string &filepath, OperandType startNumber) {
    auto [registers, program] = readData(filepath);
    auto programNumbers = programToNumbers(program);

    for (OperandType i = startNumber;; ++i) {
        registers[0] = i;
        auto result = runProgram(registers, programNumbers);
        if (result == programNumbers) {
            return i;
        }
    }
}

static auto star2SpecificCase(const std::string &filepath) {
    if (filepath != aoc_utils::INPUT_FILE) {
        throw std::runtime_error("Only implemented for: input.txt");
    }

    auto [registers, program] = readData(filepath);
    auto programNumbers = programToNumbers(program);

    std::function<int64_t(int64_t aLast, std::vector<OperandType> programNumbers)> f;
    f = [&f](int64_t aLast, std::vector<OperandType> programNumbers) -> int64_t {
        if (programNumbers.empty()) {
            return aLast;
        }

        auto lastProgramNumber = programNumbers.back();
        programNumbers.pop_back();

        // start:
        // 2,4: B = A % 8
        // 1,5: B = B ^ 5
        // 7,5: C = A >> B
        // 1,6: B = B ^ 6
        // 0,3: A = A << 3
        // 4,1: B = B ^ C
        // 5,5: print(B % 8)
        // 6,6: if (A == 0) then end else goto start

        for (int a = 0; a < 8; ++a) {
            auto aNext = (aLast << 3) + a;
            auto b = ((a ^ 3) ^ (aNext >> (a ^ 5))) % 8;
            if (b == lastProgramNumber) {
                auto currentResult = f(aNext, programNumbers);
                if (currentResult != -1) {
                    return currentResult;
                }
            }
        }

        return -1;
    };

    auto a = f(0, programNumbers);
    assert(star2BruteForce(filepath, a) == a);
    return a;
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;

    std::cout << star2BruteForce("example_input2.txt", 1) << std::endl;
    std::cout << star2SpecificCase(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}

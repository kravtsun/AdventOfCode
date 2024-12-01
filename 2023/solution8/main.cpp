#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>

using int64 = uint64_t;
template<typename T=int64> T gcd(T a, T b) {
    return b == 0? a : gcd(b, a % b);
}

template<typename T=int64> T lcm(T a, T b) {
    return a / gcd(a, b) * b;
}

int main() {
    FILE *f = freopen(WORKDIR "input.txt", "r", stdin);

    std::map<std::string, std::pair<std::string, std::string>> connections;
    std::string instructions;
    std::getline(std::cin, instructions);

    std::string line;
    std::getline(std::cin, line); // empty line.
    assert(line.empty());

    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        std::string dummy;
        std::string sourceNode, leftNode, rightNode;
        lineStream >> sourceNode;
        lineStream >> dummy; // :
        lineStream >> leftNode;
        assert (leftNode.front() == '(');
        leftNode.erase(leftNode.begin());
        assert(leftNode.back() == ',');
        leftNode.pop_back();

        lineStream >> rightNode;
        assert(rightNode.back() == ')');
        rightNode.pop_back();

        assert(connections.count(sourceNode) == 0);
        connections[sourceNode] = std::make_pair(leftNode, rightNode);
    }

    const auto singleNodeResult = [&instructions,&connections](std::string node) -> int64 {
        int result = 0;
        for (int instructionIndex = 0; node.back() != 'Z'; instructionIndex = (instructionIndex + 1) % instructions.size(), result++) {
            const auto currentInstruction = instructions[instructionIndex];
            if (currentInstruction == 'L') {
                node = connections.at(node).first;
            } else {
                assert(currentInstruction == 'R');
                node = connections.at(node).second;
            }
        }
        return result;
    };

    int64 result = 0;
    for (const auto& [sourceNode, outNodes] : connections) {
        if (sourceNode.back() == 'A') {
            if (result == 0) {
                result = singleNodeResult(sourceNode);
            } else {
                result = lcm(result, singleNodeResult(sourceNode));
            }
        }
    }

    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cassert>
#include <map>
#include <sstream>
#include <queue>
#include <set>
#include <algorithm>

enum struct Operation {
    AND = 0,
    OR,
    XOR
};

struct Edge {
    std::string to; // co-operand
    Operation op;
    std::string outputName;

    Edge(std::string to, Operation op, std::string outputName) : to(std::move(to)),
                                                                 op(op),
                                                                 outputName(std::move(outputName)) {}
};

using Node = std::string;
using NodeValues = std::map<std::string, bool>;
using Edges = std::map<Node, std::vector<Edge>>;

static auto stringToOperation(const std::string &opString) {
    Operation op;
    if (opString == "AND") {
        op = Operation::AND;
    } else if (opString == "OR") {
        op = Operation::OR;
    } else if (opString == "XOR") {
        op = Operation::XOR;
    } else {
        throw std::runtime_error("Wrong operation string: " + opString);
    }
    return op;
}

static auto readInput(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());

    std::map<Node, bool> nodeValues;

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) break;
        auto delimiterPos = line.find(':');
        assert(delimiterPos != std::string::npos);
        auto nodeName = line.substr(0, delimiterPos);
        nodeValues[nodeName] = std::stoi(line.substr(delimiterPos + 1));
    }

    Edges edges;
    while (std::getline(fin, line)) {
        std::istringstream is{line};
        Node a, b;
        std::string opString;
        is >> a >> opString >> b;
        auto op = stringToOperation(opString);
        Node outputName;
        is >> outputName;
        assert(outputName == "->");
        is >> outputName;
        edges[a].emplace_back(b, op, outputName);
        edges[b].emplace_back(a, op, outputName);
    }
    return std::make_tuple(nodeValues, edges);
}

static auto valueFromPrefix(const std::map<Node, bool> &nodeValues, char prefix) {
    uint64_t result = 0;
    for (auto it = nodeValues.rbegin(); it != nodeValues.rend(); ++it) {
        auto [name, value] = *it;
        if (name.front() == prefix) {
            result <<= 1;
            if (value) {
                result++;
            }
        }
    }
    return result;
}

static auto star1(NodeValues nodeValues, const Edges &edges) {
    std::queue<std::tuple<Node, bool>> q;
    for (auto [name, value]: nodeValues) {
        q.emplace(name, value);
    }

    while (!q.empty()) {
        auto [fromName, fromValue] = q.front();
        q.pop();
        if (edges.count(fromName) == 0) continue;
        for (auto [toName, op, outName]: edges.at(fromName)) {
            if (nodeValues.count(outName) || nodeValues.count(toName) == 0) continue;
            auto toValue = nodeValues.at(toName);
            bool outValue;
            switch (op) {
                case Operation::AND:
                    outValue = fromValue && toValue;
                    break;
                case Operation::OR:
                    outValue = fromValue || toValue;
                    break;
                case Operation::XOR:
                    outValue = fromValue ^ toValue;
                    break;
                default:
                    assert(false);
            }
            nodeValues[outName] = outValue;
            q.emplace(outName, outValue);
        }
    }

    return valueFromPrefix(nodeValues, 'z');
}

static auto star1(const std::string &filepath) {
    auto [nodeValues, edges] = readInput(filepath);
    return star1(nodeValues, edges);
}

static auto star2(const std::string &filepath) {
    auto [nodeValues, edges] = readInput(filepath);

    // reverse edges: output node name to its parents
    const auto reverseEdges = [](const Edges &forwardEdges) {
        std::map<Node, std::tuple<Node, Node, Operation>> outputToOperation;
        std::map<std::tuple<Node, Node, Operation>, Node> operationToOutput;

        for (const auto &[fromName, fromConnections]: forwardEdges) {
            for (const auto &[to, op, outputName]: fromConnections) {
                outputToOperation[outputName] = std::make_tuple(fromName, to, op);
                operationToOutput[outputToOperation[outputName]] = outputName;
            }
        }
        return std::make_tuple(outputToOperation, operationToOutput);
    };

    auto [outputToOperation, operationToOutput] = reverseEdges(edges);

    const auto indexNumberString = [](int num) {
        auto result = std::to_string(num);
        if (result.size() == 1) {
            result = "0" + result;
        }
        return result;
    };

    std::vector<std::string> swapNodes;
    const auto swapOperationsIfNeeded = [&](const std::string &firstOutput, const std::string &secondOutput) {
        if (firstOutput == secondOutput) return;

        swapNodes.push_back(firstOutput);
        swapNodes.push_back(secondOutput);

        auto &firstParameters = outputToOperation.at(firstOutput);
        assert(operationToOutput[firstParameters] == firstOutput);
        auto &secondParameters = outputToOperation.at(secondOutput);
        assert(operationToOutput[secondParameters] == secondOutput);

        operationToOutput[firstParameters] = secondOutput;
        operationToOutput[secondParameters] = firstOutput;
        std::swap(std::get<0>(firstParameters), std::get<1>(firstParameters));
        std::swap(std::get<0>(secondParameters), std::get<1>(secondParameters));
        operationToOutput[firstParameters] = secondOutput;
        operationToOutput[secondParameters] = firstOutput;
        std::swap(firstParameters, secondParameters);
    };

    const auto getOutput = [&](std::string a, std::string b, Operation op) -> std::string {
        auto parameters = std::make_tuple(a, b, op);
        if (operationToOutput.count(parameters) != 0) {
            return operationToOutput.at(parameters);
        }

        const auto emptyNodeIfNotExist = [&edges](std::string &a) {
            if (!edges.count(a) && !a.empty() && a.front() != 'z') {
                a = "";
            }
        };

        emptyNodeIfNotExist(a);
        emptyNodeIfNotExist(b);
        if (a.empty() || b.empty()) {
            auto nonEmptyInput = a.empty() ? b : a;
            switch (op) {
                case Operation::AND:
                    return "";
                case Operation::OR:
                case Operation::XOR:
                    return nonEmptyInput;
                default:
                    throw std::runtime_error("Wrong operation");
            }
        }

        std::array inputNodes{a, b};
        for (int i = 0; i < inputNodes.size(); ++i) {
            const auto &node = inputNodes[i];
            auto it = operationToOutput.lower_bound(std::make_tuple(node, "", op));
            if (it == operationToOutput.end()) continue;
            while (it != operationToOutput.end() && std::get<0>(it->first) == node) {
                if (std::get<2>(it->first) == op) {
                    swapOperationsIfNeeded(inputNodes[(i + 1) % 2], std::get<1>(it->first));
                    return it->second;
                }
                it++;
            }
        }

        throw std::runtime_error("Swap node not found");
    };

    // z00 = (x00 ^ y00)
    // z01 = (x01 ^ y01) ^ prt@(x00&y00)
    // z02 = (x02 ^ y02) ^ qnf@( (x01&y01) | ( (x01^y01) & prt@(x00&y00)) )
    // z03 = (x03 ^ y03) ^ vnm@( (x02&y02) | ( (x02^y02) & qnf ) )
    // z04 = (x04 ^ y04) ^ jdk@( (x03&y03) | ( (x03^y03) & vnm ) )
    Node previousRightPart;
    for (auto i = 0; i < 63; ++i) {
        const auto indexString = indexNumberString(i);
        const auto zName = "z" + indexString;
        if (outputToOperation.count(zName) == 0) break;

        const auto xName = "x" + indexString;
        const auto yName = "y" + indexString;
        const auto xPrevName = "x" + indexNumberString(i - 1);
        const auto yPrevName = "y" + indexNumberString(i - 1);

        // x_i ^ y_i, operand in ^ (current z), & (next z)
        const auto currentXor = getOutput(xName, yName, Operation::XOR);
        // x_{i-1} ^ y_{i-1}
        const auto previousXor = getOutput(xPrevName, yPrevName, Operation::XOR);

        // x_{i-1} & y_{i-1}, operand in |
        auto previousAnd = getOutput(xPrevName, yPrevName, Operation::AND);

        // operand in |
        auto andOperation = getOutput(previousXor, previousRightPart, Operation::AND);

        // operand in ^ (current z), & (next z)
        auto orOperation = getOutput(previousAnd, andOperation, Operation::OR);
        previousRightPart = orOperation;

        auto outerXor = getOutput(currentXor, orOperation, Operation::XOR);
        swapOperationsIfNeeded(outerXor, zName);
    }

    std::sort(swapNodes.begin(), swapNodes.end());
    std::ostringstream os;
    for (int i = 0; i < swapNodes.size(); ++i) {
        if (i > 0) {
            os << ",";
        }
        os << swapNodes[i];
    }
    return os.str();
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl; // 4
    std::cout << star1("example_input2.txt") << std::endl; // 2024
    std::cout << star1("input.txt") << std::endl; // 53325321422566
    std::cout << star2("input.txt") << std::endl; // fkb,nnr,rdn,rqf,rrn,z16,z31,z37
    return 0;
}

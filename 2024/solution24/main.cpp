#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <map>
#include <sstream>
#include <queue>

static auto star1(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());

    std::map<std::string, bool> nodeValues;

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) break;
        auto delimiterPos = line.find(':');
        assert(delimiterPos != std::string::npos);
        auto nodeName = line.substr(0, delimiterPos);
        nodeValues[nodeName] = std::stoi(line.substr(delimiterPos + 1));
    }

    enum Operation {
        AND,
        OR,
        XOR
    };

    std::map<std::string, std::vector<std::tuple<std::string, Operation, std::string>>> edges;

    while (std::getline(fin, line)) {
        std::istringstream is{line};
        std::string a, b;
        std::string opString;
        is >> a >> opString >> b;
        Operation op;
        if (opString == "AND") {
            op = AND;
        } else if (opString == "OR") {
            op = OR;
        } else if (opString == "XOR") {
            op = XOR;
        } else {
            assert(false);
        }
        std::string s;
        is >> s;
        assert(s == "->");
        is >> s;
        edges[a].emplace_back(b, op, s);
        edges[b].emplace_back(a, op, s);
    }

    std::queue<std::tuple<std::string, bool>> q;
    for (auto [name, value]: nodeValues) {
        q.emplace(name, value);
    }

    while (!q.empty()) {
        auto [fromName, fromValue] = q.front();
        q.pop();
        for (auto [toName, op, outName]: edges[fromName]) {
            if (nodeValues.count(outName) || nodeValues.count(toName) == 0) continue;
            auto toValue = nodeValues.at(toName);
            bool outValue;
            switch (op) {
                case AND:
                    outValue = fromValue && toValue;
                    break;
                case OR:
                    outValue = fromValue || toValue;
                    break;
                case XOR:
                    outValue = fromValue ^ toValue;
                    break;
                default:
                    assert(false);
            }
            nodeValues[outName] = outValue;
            q.emplace(outName, outValue);
        }
    }

    int64_t result = 0;
    for (auto it = nodeValues.rbegin(); it != nodeValues.rend(); ++it) {
        auto [name, value] = *it;
        if (name.front() == 'z') {
            result <<= 1;
            if (value) {
                result++;
            }
        }
    }
    return result;
}

int main() {
    std::cout << star1("example_input1.txt") << std::endl;
    std::cout << star1("example_input2.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;

    return 0;
}

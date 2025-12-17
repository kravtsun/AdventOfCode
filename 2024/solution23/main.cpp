#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>

using Node = std::string;
using Nodes = std::set<std::string>;
using Edges = std::map<Node, std::set<Node>>;

static auto readEdges(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    Edges edges;
    std::string line;
    while (std::getline(fin, line)) {
        assert(line.size() == 5);
        auto a = line.substr(0, 2);
        auto b = line.substr(3, 2);
        assert(line[2] == '-');
        edges[a].insert(b);
        edges[b].insert(a);
    }
    return edges;
}

static auto star1(const std::string &filepath) {
    const auto edges = readEdges(filepath);
    int result = 0;
    for (const auto &[a, aConnections]: edges) {
        for (auto bIt = aConnections.upper_bound(a); bIt != aConnections.end(); ++bIt) {
            const auto &b = *bIt;
            assert(edges.at(b).count(a));
            for (auto cIt = std::next(bIt); cIt != aConnections.end(); ++cIt) {
                const auto &c = *cIt;
                assert(edges.at(c).count(a));
                bool isClique = edges.at(b).count(c);
                assert(!isClique || edges.at(c).count(b));
                if (!isClique) continue;
                bool canBeBossAround = a.front() == 't' || bIt->front() == 't' || cIt->front() == 't';
                if (canBeBossAround) {
                    result++;
                }
            }
        }
    }
    return result;
}


static Nodes DFS(const Edges &edges, const Nodes &possibleNodes) {
    const auto getPossibleConnections = [&](const Node &a) {
        // Only proceed further by increasing node strings lexigraphically
        const auto &aConnections = edges.at(a);

        Nodes aPossibleConnections;
        std::set_intersection(aConnections.upper_bound(a), aConnections.end(),
                              possibleNodes.upper_bound(a), possibleNodes.end(),
                              std::inserter(aPossibleConnections, aPossibleConnections.end()));
        return aPossibleConnections;
    };

    Edges possibleEdges;
    for (const auto &a: possibleNodes) {
        possibleEdges[a] = getPossibleConnections(a);
    }

    Nodes bestClique;
    for (const auto &a: possibleNodes) {
        const auto &aPossibleConnections = possibleEdges.at(a);
        if (aPossibleConnections.empty() && bestClique.empty()) {
            bestClique = {a};
        }

        for (const auto &b: aPossibleConnections) {
            assert(possibleNodes.count(b) > 0);
            const auto &bPossibleConnections = possibleEdges.at(b);

            // We assume that there is no need to include any node between a and b -
            // since they have already been processed here before b.
            Nodes abPossibleConnections;
            std::set_intersection(aPossibleConnections.begin(), aPossibleConnections.end(),
                                  bPossibleConnections.begin(), bPossibleConnections.end(),
                                  std::inserter(abPossibleConnections, abPossibleConnections.end()));

            auto nextBestClique = DFS(possibleEdges, abPossibleConnections);
            nextBestClique.insert(nextBestClique.begin(), b);
            nextBestClique.insert(nextBestClique.begin(), a);
            if (nextBestClique.size() > bestClique.size()) {
                bestClique = nextBestClique;
            }
        }
    }
    return bestClique;
}

static auto star2(const std::string &filepath) {
    const auto edges = readEdges(filepath);
    Nodes allNodes;
    for (const auto &[a, _]: edges) {
        allNodes.insert(allNodes.begin(), a);
    }

    auto bestClique = DFS(edges, allNodes);
    std::string bestCliqueString;
    for (const auto &a: bestClique) {
        bestCliqueString += a + ",";
    }
    assert(bestCliqueString.back() == ',');
    bestCliqueString.pop_back();
    return bestCliqueString;
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // 7
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl; // 1175
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl; // co,de,ka,ta
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl; // bw,dr,du,ha,mm,ov,pj,qh,tz,uv,vq,wq,xw
    return 0;
}

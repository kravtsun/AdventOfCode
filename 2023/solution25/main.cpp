#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <memory>
#include <numeric>
#include <queue>
#include <set>

using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pdd = std::pair<double, double>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
using vpii = std::vector<pii>;
using spii = std::set<pii>;
using vs = std::vector<std::string>;
using vbool = std::vector<bool>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

int main() {
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
//    std::ifstream fin{WORKDIR "input_sample.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::map<std::string, int> nameToIndex;
    std::vector<std::string> indexToName;
    vvint g;

    const auto getIndex = [&](std::string name) -> int {
        if (nameToIndex.count(name) == 0) {
            int i = nameToIndex.size();
            nameToIndex[name] = i;
            g.resize(nameToIndex.size());
            indexToName.push_back(name);
        }
        return nameToIndex.at(name);
    };

    vpii e;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        std::string from;
        lineStream >> from;
        assert(from.back() == ':');
        from.pop_back();

        int fromIndex = getIndex(from);

        std::string to;
        while (lineStream >> to) {
            int toIndex = getIndex(to);
            g.at(fromIndex).push_back(toIndex);
            g.at(toIndex).push_back(fromIndex);
            e.emplace_back(fromIndex, toIndex);
        }
    }

    const int n = g.size();

    const pii invalidEdge{-1, -1};
    vbool used(n, false);
    vint tin(n), fup(n);
    const auto find_bridge = [&]() -> pii {
        // https://e-maxx.ru/algo/bridge_searching
        used.assign(n, false);
        int timer = 0;

        std::function<void(int, int)> DFS;

        std::vector<pii> bridges;

        DFS = [&](int v, int p = -1) -> void {
            used[v] = true;
            tin[v] = fup[v] = timer++;
            for (size_t i=0; i<g[v].size(); ++i) {
                int to = g[v][i];
                if (to == p)  continue;
                if (used[to])
                    fup[v] = std::min (fup[v], tin[to]);
                else {
                    DFS(to, v);
                    fup[v] = std::min (fup[v], fup[to]);
                    if (fup[to] > tin[v]) {
                        bridges.emplace_back(v, to);
                    }
                }
            }
        };

        DFS(0, -1);
        const auto usedCount = std::count(used.begin(), used.end(), true);
        assert(usedCount == used.size());

        if (bridges.empty()) {
            return invalidEdge;
        } else if (bridges.size() == 1) {
            return bridges.front();
        } else {
            assert(false);
        }
    };

    const auto removeEdge = [](const pii& edge, vvint& g) {
        auto it1 = std::find(g[edge.first].begin(), g[edge.first].end(), edge.second);
        g[edge.first].erase(it1);

        auto it2 = std::find(g[edge.second].begin(), g[edge.second].end(), edge.first);
        g[edge.second].erase(it2);
    };

    const auto addEdge = [](const pii& edge, vvint& g) {
        g[edge.first].push_back(edge.second);
        g[edge.second].push_back(edge.first);
    };

    for (int e1 = 0; e1 < e.size(); ++e1) {
        const auto edge1 = e[e1];
        removeEdge(edge1, g);

        for (int e2 = e1 + 1; e2 < e.size(); ++e2) {
            const auto edge2 = e[e2];
            removeEdge(edge2, g);
            const auto bridge = find_bridge();
            if (bridge != invalidEdge) {
                removeEdge(bridge, g);
                vbool used(n, false);
                std::queue<int> q;
                q.push(0);
                used[0] = true;
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();
                    for (auto to : g[v]) {
                        if (!used[to]) {
                            q.push(to);
                            used[to] = true;
                        }
                    }
                }


                std::cout << indexToName[edge1.first] << " " << indexToName[edge1.second] << std::endl;
                std::cout << indexToName[edge2.first] << " " << indexToName[edge2.second] << std::endl;
                std::cout << indexToName[bridge.first] << " " << indexToName[bridge.second] << std::endl;

                const auto firstComponentSize = std::count(used.begin(), used.end(), true);
                const auto secondComponentSize = used.size() - firstComponentSize;
                std::cout << firstComponentSize * secondComponentSize << std::endl;
                return 0;
            }
            addEdge(edge2, g);
        }
        addEdge(edge1, g);
    }




    return 0;
}
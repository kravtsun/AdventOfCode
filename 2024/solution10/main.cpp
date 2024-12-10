#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <map>
#include <queue>
#include <set>

using pii = std::pair<int, int>;
static bool isGoodCoordinate(int x, int n) {
    return 0 <= x && x < n;
}
static bool isGoodPoint(pii p, int n, int m) {
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static auto read_lines(std::istream &fin) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

struct Vertex {
    int i;
    int j;
    size_t height;
    size_t index;

    Vertex(int i, int j, size_t height, size_t index) : i(i), j(j), height(height), index(index) {}
};

static auto makeGraph(const std::vector<std::string> &lines) {
    const int n = static_cast<int>(lines.size());
    const int m = static_cast<int>(lines[0].size());
    std::vector<Vertex> vertexes;
    std::map<pii, size_t> pointToVertexIndex;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (!isdigit(lines[i][j])) continue;
            const auto vertexIndex = vertexes.size();
            vertexes.emplace_back(i, j, static_cast<size_t>(lines[i][j] - '0'), vertexIndex);
            pointToVertexIndex[std::make_pair(i, j)] = vertexIndex;
        }
    }

    const std::array di{-1, 1, 0, 0};
    const std::array dj{0, 0, -1, 1};
    static_assert(di.size() == dj.size());

    const auto V = vertexes.size();
    std::vector<std::vector<size_t>> edges(V);
    for (int iv = 0; iv < V; ++iv) {
        const auto &v = vertexes[iv];
        const pii p{v.i, v.j};

        for (int k = 0; k < di.size(); ++k) {
            pii pNeighbor{p.first + di[k], p.second + dj[k]};
            if (!pointToVertexIndex.count(pNeighbor)) continue;
            auto vNeighbor = vertexes[pointToVertexIndex.at(pNeighbor)];
            if (vNeighbor.height == v.height + 1) {
                edges[iv].push_back(vNeighbor.index);
            }
        }
    }
    return std::make_tuple(vertexes, edges);
}

static int star(const std::vector<std::string> &lines, bool isStar2) {
    auto [vertexes, edges] = makeGraph(lines);
    const auto V = vertexes.size();

    std::vector<int> weights(V);
    std::queue<Vertex> q;

    int result = 0;
    for (auto v0 : vertexes) {
        if (v0.height != 0) continue;
        weights.assign(V, 0);
        assert(q.empty());
        q.emplace(v0);
        weights[v0.index] = 1;
        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            if (v.height == 9) {
                result += weights[v.index];
                assert(edges[v.index].empty());
                continue;
            }
            for (auto iNeighbor : edges[v.index]) {
                if (weights[iNeighbor] == 0) {
                    q.push(vertexes[iNeighbor]);
                    weights[iNeighbor] += weights[v.index];
                } else if (isStar2) {
                    weights[iNeighbor] += weights[v.index];
                }
            }
        }
    }
    return result;
}

static int solve(const std::string &filename, bool isStar2) {
    std::ifstream fin{filename};
    assert(fin.is_open());
    auto lines = read_lines(fin);
    return star(lines, isStar2);
}

static void test_star1() {
    std::map<int, int> expectedResults = {
            {1, 1},
            {2, 2},
            {3, 4},
            {4, 3},
            {5, 36}
    };

    for (auto [exampleIndex, expectedResult] : expectedResults) {
        const auto actualResult = solve("example_input" + std::to_string(exampleIndex) + ".txt", false);
        std::cout << exampleIndex << " -> " << actualResult << std::endl;
        assert(expectedResult == actualResult);
    }

    std::cout << solve("input.txt", false) << std::endl;
}

static void test_star2() {
    std::map<int, int> expectedResults = {
            {6, 3},
            {3, 13},
            {7, 227},
            {5, 81},
    };

    for (auto [exampleIndex, expectedResult] : expectedResults) {
        const auto actualResult = solve("example_input" + std::to_string(exampleIndex) + ".txt", true);
        std::cout << exampleIndex << " -> " << actualResult << std::endl;
        assert(expectedResult == actualResult);
    }

    std::cout << solve("input.txt", true) << std::endl;
}

int main() {
    test_star1();
    test_star2();

    return 0;
}

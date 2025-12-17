#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <numeric>

using Point = std::pair<int, int>;

static auto readLines(std::istream &fin) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

struct Vertex {
    size_t i;
    size_t j;
    size_t height;
    size_t index;

    Vertex(int i, int j, size_t height, size_t index) : i(i), j(j), height(height), index(index) {}
};

static auto makeGraph(const std::vector<std::string> &lines) {
    const auto n = lines.size();
    const auto m = lines[0].size();
    std::vector<Vertex> vertexes;
    std::map<Point, size_t> pointToVertexIndex;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
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
        const Point p{v.i, v.j};

        for (int k = 0; k < di.size(); ++k) {
            Point pNeighbor{p.first + di[k], p.second + dj[k]};
            if (!pointToVertexIndex.count(pNeighbor)) continue;
            auto vNeighbor = vertexes[pointToVertexIndex.at(pNeighbor)];
            if (vNeighbor.height == v.height + 1) {
                edges[iv].push_back(vNeighbor.index);
            }
        }
    }
    return std::make_tuple(vertexes, edges);
}

static void DFS(const std::vector<Vertex> &vertexes,
                const std::vector<std::vector<size_t>> &edges,
                std::map<size_t, std::set<size_t>> &beacons,
                size_t vIndex) {
    const int MAX_HEIGHT = 9;
    const auto &v = vertexes[vIndex];
    if (v.height == MAX_HEIGHT) {
        beacons[vIndex].insert(vIndex);
    }
    beacons[vIndex];

    const auto &tos = edges.at(vIndex);
    for (auto to: tos) {
        if (beacons.count(to) == 0) {
            DFS(vertexes, edges, beacons, to);
        }
        const auto newBeacons = beacons.at(to);
        for (auto b: newBeacons) {
            beacons[vIndex].insert(b);
        }
    }
}

static auto solveStar1(const std::vector<std::string> &lines) {
    auto [vertexes, edges] = makeGraph(lines);
    std::map<size_t, std::set<size_t>> beacons;
    size_t result = 0;
    for (size_t i = 0; i < vertexes.size(); ++i) {
        assert(vertexes[i].index == i);
        if (vertexes[i].height == 0) {
            DFS(vertexes, edges, beacons, i);
            result += beacons.at(i).size();
        }
    }
    return result;
}

static auto solveStar2(const std::vector<std::string> &lines) {
    auto [vertexes, edges] = makeGraph(lines);
    std::map<size_t, std::vector<Vertex>> heightToVertex;
    for (auto v: vertexes) {
        heightToVertex[v.height].push_back(v);
    }

    std::vector<int> weights(vertexes.size());
    for (const auto &[height, heightVertexes]: heightToVertex) {
        if (height == heightToVertex.begin()->first) {
            for (const auto &v: heightVertexes) {
                weights[v.index] = 1;
            }
        }
        for (const auto &v: heightVertexes) {
            for (auto toIndex: edges[v.index]) {
                weights[toIndex] += weights[v.index];
            }
        }
    }

    return std::accumulate(heightToVertex.rbegin()->second.cbegin(), heightToVertex.rbegin()->second.cend(), 0UL,
                           [&weights](int init, const Vertex &v) {
                               return init + weights[v.index];
                           });
}

static auto solve(const std::string &filename, bool isStar2) {
    std::ifstream fin{filename};
    assert(fin.is_open());
    auto lines = readLines(fin);
    return isStar2 ? solveStar2(lines) : solveStar1(lines);
}

static void test_star1() {
    std::map<int, int> expectedResults = {
            {1, 1},
            {2, 2},
            {3, 4},
            {4, 3},
            {5, 36}
    };

    for (auto [exampleIndex, expectedResult]: expectedResults) {
        const auto actualResult = solve("example_input" + std::to_string(exampleIndex) + ".txt", false);
        std::cout << exampleIndex << " -> " << actualResult << std::endl;
        assert(expectedResult == actualResult);
    }

    std::cout << solve(aoc_utils::INPUT_FILE, false) << std::endl;
}

static void test_star2() {
    std::map<int, int> expectedResults = {
            {6, 3},
            {3, 13},
            {7, 227},
            {5, 81},
    };

    for (auto [exampleIndex, expectedResult]: expectedResults) {
        const auto actualResult = solve("example_input" + std::to_string(exampleIndex) + ".txt", true);
        std::cout << exampleIndex << " -> " << actualResult << std::endl;
        assert(expectedResult == actualResult);
    }

    std::cout << solve(aoc_utils::INPUT_FILE, true) << std::endl;
}

int main() {
    test_star1();
    test_star2();
    return 0;
}

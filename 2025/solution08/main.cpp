// This software was partially written using Suggestions from GitHub Copilot.

#include <algorithm>
#include <utility>
#include <numeric>
#include <functional>
#include <map>

#include "aoc_utils/io_helpers.h"
#include "aoc_utils/point.h"

template <typename T>
static auto INF = std::numeric_limits<T>::max();

using Coordinate = std::uint64_t;

static auto read_input(const std::string &filename) {
    const auto filepath = aoc_utils::get_input_filepath(filename, 8);
    using Point3 = aoc_utils::PointTmpl<Coordinate, 3>;
    std::vector<Point3> points;

    auto lines = aoc_utils::read_lines(filepath);
    for (const auto &line : lines) {
        std::istringstream is(line);
        char c;
        Coordinate x, y, z;
        is >> x >> c >> y >> c >> z;
        points.emplace_back(x, y, z);
    }
    return points;
}

/// @param k set k to infinity for the second star.
static auto star(const std::string &filename, int k = INF<int>) {
    const auto isSecondStar = k == INF<int>;
    auto points = read_input(filename);

    using Index = std::size_t;
    std::vector<Index> dsu(points.size());
    std::iota(dsu.begin(), dsu.end(), 0);

    std::function<Index(Index)> getCC;
    getCC = [&dsu, &getCC](Index i) {
        if (dsu[i] == i) return i;
        return dsu[i] = getCC(dsu[i]);
    };

    using Distance = Coordinate;
    using IndexPair = std::pair<Index, Index>;
    std::map<Distance, IndexPair> distances;

    for (Index i = 0; i < points.size(); ++i) {
        for (Index j = i + 1; j < points.size(); ++j) {
            auto curDist = points[i].distSquared(points[j]);
            distances[curDist] = std::make_pair(i, j);
        }
    }

    using Sizes = std::vector<std::size_t>;

    std::map<IndexPair, bool> connected;
    while (isSecondStar || k--) {
        auto [bestI, bestJ] = distances.begin()->second;
        distances.erase(distances.begin());

        auto iCC = getCC(bestI);
        auto jCC = getCC(bestJ);
        dsu[iCC] = jCC;
        connected[IndexPair(bestI, bestJ)] = true;

        if (isSecondStar) {
            Sizes sizes(points.size(), 0);
            for (int i = 0; i < points.size(); ++i) {
                sizes[getCC(i)]++;
            }
            if (*max_element(sizes.begin(), sizes.end()) == points.size()) {
                return static_cast<std::uint64_t>(points[bestI][0]) * points[bestJ][0];
            }
        }
    }

    Sizes sizes(points.size(), 0);
    for (int i = 0; i < points.size(); ++i) {
        sizes[getCC(i)]++;
    }

    std::sort(sizes.begin(), sizes.end(), std::greater<>());

    return static_cast<std::uint64_t>(sizes[0]) * sizes[1] * sizes[2];
}

static auto star1(const std::string &filepath, int k) {
    return star(filepath, k);
}

static auto star2(const std::string &filepath) {
    return star(filepath, INF<int>);
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE, 10) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE, 1000) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}

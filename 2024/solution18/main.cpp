#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <cassert>
#include <map>

using Point = std::pair<int, int>;

static Point addPoint(const Point &lhs, const Point &rhs) {
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

static bool isGoodPoint(const Point &p, int n, int m) {
    static constexpr auto isGoodCoordinate = [](int x, int n) {
        return 0 <= x && x < n;
    };
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

static auto readPoints(const std::string &filepath) {
    std::ifstream fin{filepath};
    std::vector<Point> points;
    int x, y;
    char delimiter;
    while (fin >> x >> delimiter >> y) {
        assert(delimiter == ',');
        points.emplace_back(x, y);
    }
    return points;
}

static auto findPath(const std::vector<std::string> &field) {
    const Point BAD_POINT{-1, -1};

    const auto height = static_cast<int>(field.size());
    const auto width = static_cast<int>(field[0].size());

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    const auto ndirs = 4;

    const Point startPos{0, 0};
    const Point finishPos{width - 1, height - 1};
    // coordinates, number of states
    std::queue<Point> q;
    std::map<Point, Point> parent;
    q.push(startPos);
    parent[startPos] = BAD_POINT;

    while (!q.empty()) {
        auto p = q.front();
        q.pop();

        if (p == finishPos) {
            std::vector<Point> path;
            while (p != BAD_POINT) {
                path.push_back(p);
                p = parent.at(p);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int k = 0; k < ndirs; ++k) {
            auto pNext = addPoint(p, {dx[k], dy[k]});
            if (isGoodPoint(pNext, width, height) && field[pNext.second][pNext.first] != '#' && !parent.count(pNext)) {
                parent[pNext] = p;
                q.emplace(pNext);
            }
        }
    }

    return std::vector<Point>{};
}

static auto star1(const std::string &filepath, int dim, int limit = -1) {
    auto points = readPoints(filepath);
    assert(points.size() >= limit);
    points.resize(limit);

    const int height = dim;
    const int width = dim;
    std::vector<std::string> field(height, std::string(width, '.'));

    for (auto [x, y]: points) {
        field[y][x] = '#';
    }

    auto path = findPath(field);
    assert(!path.empty());
    return path.size() - 1;
}

static auto star2(const std::string &filepath, int dim) {
    auto points = readPoints(filepath);
    const int height = dim;
    const int width = dim;
    std::vector<std::string> field(height, std::string(width, '.'));

    auto path = findPath(field);
    assert(!path.empty());

    std::set<Point> pathPoints{path.begin(), path.end()};

    for (auto p: points) {
        field[p.second][p.first] = '#';
        if (!pathPoints.count(p)) {
            continue;
        }
        path = findPath(field);
        if (path.empty()) {
            std::ostringstream os;
            os << p.first << "," << p.second;
            return os.str();
        }

        pathPoints.clear();
        std::copy(path.begin(), path.end(), std::inserter(pathPoints, pathPoints.end()));
        pathPoints = std::set<Point>{path.begin(), path.end()};
    }
    return std::string("FAIL");
}

int main() {
    std::cout << star1("example_input.txt", 7, 12) << std::endl; // 22
    std::cout << star1("input.txt", 71, 1024) << std::endl; // 318
    std::cout << star2("example_input.txt", 7) << std::endl; // 6,1
    std::cout << star2("input.txt", 71) << std::endl; // 56,29
    return 0;
}

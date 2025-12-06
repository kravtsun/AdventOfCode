// This software was partially written using Suggestions from GitHub Copilot.
#include <bits/stdc++.h>

/// Reads all lines from a file and returns them as a vector of strings.
static auto read_lines(const std::string &filepath) {
    std::ifstream fin{filepath};
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open the file: " + filepath);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

using Point = std::pair<int, int>;

static bool is_good_point(const Point &p, int n, int m) {
    return p.first >= 0 && p.first < n && p.second >= 0 && p.second < m;
}

static auto add_point(const Point &lhs, const Point &rhs) {
    return Point{lhs.first + rhs.first, lhs.second + rhs.second};
}

static auto get_neighbor_rolls(const std::vector<std::string> &lines, const Point &p) {
    // in which directions there can be neighbors
    static const int deltas[3] = {-1, 0, 1};

    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    std::vector<Point> neighborRolls;
    for (auto dy : deltas) {
        for (auto dx : deltas) {
            if (dy == 0 && dx == 0) {
                // do not count the roll of paper itself as its own neighbor
                continue;
            }

            const Point pNext = add_point(p, {dy, dx});
            if (!is_good_point(pNext, n, m)) continue;
            bool isNeighborRoll = lines[pNext.first][pNext.second] == '@';
            if (isNeighborRoll) {
                neighborRolls.push_back(pNext);
            }
        }
    }
    return neighborRolls;
}

static auto star1(const std::string &filepath) {
    const auto lines = read_lines(filepath);
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());
    int res = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] != '@') {
                // Continue if we do not stand now upon a roll of paper
                continue;
            }
            Point p{i, j};
            auto cnt = get_neighbor_rolls(lines, p).size();
            if (cnt < 4) {
                // check if a roll is accessible
                res++;
            }
        }
    }
    return res;
}

static auto star2(const std::string &filepath) {
    auto lines = read_lines(filepath);
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

    int res = 0;

    // Running breadth-first search
    // in the processing queue q we put a roll cell when it can change its state
    std::queue<Point> q;

    // as `used[p] = true` we mark the points which are:
    // - to be processed on the next iteration (wave) - so that we do not add into the queue them twice,
    // - if we are not going to process them anymore (turned into a non-roll state), not putting them into queue
    std::map<Point, bool> used;

    // returns true if flip successful.
    const auto try_flip_roll = [&lines, &q, &used, n, m](const Point &p) {
        auto neighbor_rolls = get_neighbor_rolls(lines, p);
        if (neighbor_rolls.size() < 4) {
            for (auto pNext : neighbor_rolls) {
                if (!used[pNext]) {
                    used[pNext] = true;
                    q.push(pNext);
                }
            }
            lines[p.first][p.second] = 'x';
            used[p] = true;
            return true;
        } else {
            used[p] = false;
            return false;
        }
    };

    // Initialize the queue with roll neighbors of the roll-cells with fewer than 4 neighbors
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] != '@') {
                // Continue if we do not stand now upon a roll of paper
                continue;
            }
            Point p{i, j};
            res += try_flip_roll(p);
        }
    }

    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        if (lines[p.first][p.second] == 'x') {
            continue;
        }

        if (lines[p.first][p.second] != '@') {
            throw std::runtime_error("The point in the BFS queue should be a roll");
        }

        auto neighbor_rolls = get_neighbor_rolls(lines, p);

        if (neighbor_rolls.size() < 4) {
            for (auto pNext : neighbor_rolls) {
                if (!used[pNext]) {
                    used[pNext] = true;
                    q.push(pNext);
                }
            }
            lines[p.first][p.second] = 'x';
            used[p] = true;
            res++;
        } else {
            used[p] = false;
        }
    }

    return res;
}

int main() {
    std::cout << star1("example_input.txt") << std::endl;
    std::cout << star1("input.txt") << std::endl;
    std::cout << star2("example_input.txt") << std::endl;
    std::cout << star2("input.txt") << std::endl;
    return 0;
}

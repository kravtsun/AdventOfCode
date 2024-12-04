#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <queue>
#include <set>
#include <map>
#include <functional>

using pii = std::pair<int, int>;

struct State {
    int i;
    int j;
    int wordIndex;
    int weight; // how many path have lead to that state
    std::vector<pii> parents;
    friend bool operator<(const State &lhs, const State &rhs) {
        if (lhs.i != rhs.i)
            return lhs.i < rhs.i;
        if (lhs.j != rhs.j)
            return lhs.j < rhs.j;
        if (lhs.wordIndex != rhs.wordIndex)
            return lhs.wordIndex < rhs.wordIndex;
//        if (lhs.weight != rhs.weight)
//            return lhs.weight < rhs.weight;
        return false;
    }
};

std::vector<std::string> field;
std::vector<std::string> lines;
static void print_point(int i, int j) {
    field[i][j] = lines[i][j];
    std::cout << i << "," << j;
}

static void print_state(const State &v) {
    print_point(v.i, v.j);
    std::cout << " <- ";
    for (auto [pi, pj] : v.parents) {
        print_point(pi, pj);
        std::cout << ";";
    }
    std::cout << std::endl;
}

int main() {
    std::ifstream fin{"example_input.txt"};
    assert(fin.is_open());

    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    for (const auto &l : lines) {
        field.emplace_back(l.size(), '.');
    }

    const std::string sample = "XMAS";
//    const std::string sample = "SAMX";
    const auto n = static_cast<int>(lines.size());
    const auto m = static_cast<int>(lines[0].size());

//    using pii = std::pair<int, int>;
    const auto BFS = [&lines, &n, &m, &sample](int i, int j) {
        State s{i, j, 0, 1, {}};
        std::queue<State> q;
        q.push(s);
        std::set<State> used;
        used.insert(s);
        std::map<pii, State> states;

        std::function<void(const State &v)> print_path;
        print_path = [&states, &print_path](const State &v) {
            print_state(v);
            for (auto [pi, pj] : v.parents) {
                print_path(states.at(std::make_pair(pi, pj)));
            }
        };

        int result = 0;
        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            states[std::make_pair(v.i, v.j)] = v;
            if (v.wordIndex + 1 == sample.size()) {
                print_path(v);
                std::cout << std::endl;
                return v.weight;
            }

            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    State newState{v.i + di, v.j + dj, v.wordIndex + 1, 1, {std::make_pair(v.i, v.j)}};
                    if (newState.i < 0 || newState.i >= n) continue;
                    if (newState.j < 0 || newState.j >= m) continue;
                    if (lines[newState.i][newState.j] != sample[newState.wordIndex]) continue;
                    auto it = used.find(newState);
                    if (it != used.end()) {
                        auto usedState = *it;
                        used.erase(it);
                        usedState.weight += v.weight;
                        usedState.parents.emplace_back(v.i, v.j);
                        used.insert(usedState);
                    } else {
                        used.insert(newState);
                        q.push(newState);
                    }
                }
            }
        }
        return result;
    };

    int result = 0;
    for (int i = 0; i < n; ++i) {
        assert(lines[i].size() == m);
        for (int j = 0; j < m; ++j) {
            if (lines[i][j] == sample.front()) {
                result += BFS(i, j);
            }
        }
    }
    std::cout << result << std::endl;

    for (const auto &fl : field) {
        std::cout << fl << std::endl;
    }

    return 0;
}

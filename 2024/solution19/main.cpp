#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <cassert>
#include <memory>

// suffix tree
struct Node {
    bool isFinal = false;
    std::map<char, std::shared_ptr<Node>> edges;
};

/**
 * Append string @param s to suffix tree (adding symbols from back)
 * @param root root of the suffix tree
 * @param s string to be added
 */
static void addString(std::shared_ptr<Node> &root, std::string s) {
    assert(root);
    if (s.empty()) {
        root->isFinal = true;
    } else {
        auto tos = root->edges.lower_bound(s.back());
        if (tos == root->edges.end() || tos->first != s.back()) {
            tos = root->edges.insert(tos, {s.back(), std::make_shared<Node>()});
        }
        s.pop_back();
        addString(tos->second, s);
    }
}

/**
 * Recursive function of finding string in a suffix tree
 * @param memo memoization structure to provide cut-off for recursion
 * @param root root of the suffix tree
 * @param v current vertex of the tree in traversal
 * @param s current string to be found
 * @return 64-bit integer number of ways to find a string in the tree
 */
static int64_t stringInTree(std::map<std::string, int64_t> &memo,
                            const std::shared_ptr<Node> &root,
                            const std::shared_ptr<Node> &v,
                            std::string s) {
    assert(root && v && !root->isFinal);
    if (s.empty()) {
        return v->isFinal;
    }

    auto memoIt = memo.end();
    if (root == v) {
        memoIt = memo.lower_bound(s);
        if (memoIt != memo.end() && memoIt->first == s) {
            return memoIt->second;
        }
        memoIt = memo.insert(memoIt, {s, 0});
    }

    int64_t result = 0;

    if (v->isFinal && root->edges.count(s.back())) {
        // can finish traversing the tree and start from beginning
        result += stringInTree(memo, root, root, s);
    }

    auto vto = v->edges.find(s.back());
    if (vto != v->edges.end()) {
        // can continue traversing the tree with current character.
        s.pop_back();
        result += stringInTree(memo, root, vto->second, s);
    }

    if (root == v) {
        memoIt->second = result;
    }
    return result;
}

static auto solve(const std::string &filepath, bool isStar2) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::string line;
    std::getline(fin, line);
    std::istringstream is{line};
    std::string pattern;

    // prefix tree
    auto root = std::make_shared<Node>();

    while (is >> pattern) {
        assert(!pattern.empty());
        if (pattern.back() == ',') {
            pattern.pop_back();
        }
        addString(root, pattern);
    }

    std::map<std::string, int64_t> memo;
    int64_t result = 0;
    std::getline(fin, line);
    assert(line.empty());
    while (std::getline(fin, line)) {
        auto lineResult = stringInTree(memo, root, root, line);
        result += isStar2 ? lineResult : (lineResult > 0);
    }
    return result;
}

static auto star1(const std::string &filepath) {
    return solve(filepath, false);
}

static auto star2(const std::string &filepath) {
    return solve(filepath, true);
}

int main() {
    std::cout << star1(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star1(aoc_utils::INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::EXAMPLE_INPUT_FILE) << std::endl;
    std::cout << star2(aoc_utils::INPUT_FILE) << std::endl;
    return 0;
}

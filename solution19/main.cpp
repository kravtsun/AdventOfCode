#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <set>
#include <queue>
#include <fstream>


using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
using vpii = std::vector<pii>;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

using PartCategory = char;
struct Part {
    [[nodiscard]] int get(PartCategory s) const {
        assert(categories.at(s) != -1);
        return categories.at(s);
    }

    void set(PartCategory s, int newValue) {
        assert(categories.count(s));
        categories[s] = newValue;
    }

    [[nodiscard]] int64 sum() const {
        int64 result = 0;
        for (auto [cat, value] : categories) {
            result += value;
        }
        return result;
    }

private:
    std::map<PartCategory, int> categories = {
            {'x', -1},
            {'m', -1},
            {'a', -1},
            {'s', -1},
    };

    friend std::istream& operator>>(std::istream& is, Part& part) {
        char c;
        while (is >> c && c != '{');
        for (int i = 0; i < part.categories.size(); ++i) {
            is >> c;
            assert(part.categories.count(c));
            char cat = c;
            is >> c;
            assert(c == '=');
            int value;
            is >> value;
            part.set(cat, value);
            is >> c;
            assert(c == ',' || c == '}');
        }
        return is;
    }
};


static const char LESS = '<';
static const char GREATER = '>';
struct Rule {
    PartCategory cat;
    char lessOrGreater;
    int threshold;
    std::string dst;

    [[nodiscard]] bool isLast() const {
        if (threshold != INT32_MIN)
            return false;

        assert(cat == 'x' && lessOrGreater == GREATER);
        return true;
    }

    friend std::istream& operator>>(std::istream & is, Rule& rule) {
        std::string ruleString;

        char c = 0;
        while (is >> c && c != ',' && c != '}') {
            ruleString += c;
        }

        assert(ruleString.back() != ',');
        assert(ruleString.back() != '}');
//        if (ruleString.back() == ',' || ruleString.back() == '}') {
//            ruleString.pop_back();
//        }

        if (ruleString.find(':') == std::string::npos) {
            rule.cat = 'x';
            rule.lessOrGreater = GREATER;
            rule.threshold = INT32_MIN;
            rule.dst = ruleString;
            return is;
        }

        std::istringstream ruleStream{ruleString};
        ruleStream >> rule.cat >> rule.lessOrGreater >> rule.threshold;
        assert(rule.lessOrGreater == LESS || rule.lessOrGreater == GREATER);
        ruleStream >> c;
        assert(c == ':');
        ruleStream >> rule.dst;
        assert(!rule.dst.empty());
        return is;
    }
};

static const std::string ACCEPTED = "A";
static const std::string REJECTED = "R";
struct RuleSet {
    std::string name;
    std::vector<Rule> rules;
    friend std::istream& operator>>(std::istream & is, RuleSet& ruleSet) {
        ruleSet.name.clear();
        char c = 0;
        while (is >> c && c != '{') {
            ruleSet.name += c;
        }

        while (true) {
            Rule rule;
            is >> rule;
            ruleSet.rules.push_back(rule);
            if (rule.isLast()) break;
        }

        return is;
    }
};

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::map<std::string, RuleSet> ruleSets;
    std::vector<Part> parts;

    std::string line;
    bool ruleSectionEnded = false;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            ruleSectionEnded = true;
            break;
        }

        std::istringstream lineStream{line};
        RuleSet ruleSet;
        lineStream >> ruleSet;
        ruleSets[ruleSet.name] = ruleSet;
        assert(ruleSet.name != ACCEPTED && ruleSet.name != REJECTED);
        assert(!ruleSet.name.empty());
    }

    assert(ruleSectionEnded);
    assert(ruleSets.count("in"));

    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        Part part;
        lineStream >> part;
        parts.push_back(part);
    }

    const auto isAccepted = [&ruleSets](const Part& part) -> bool {

        RuleSet ruleSet = ruleSets.at("in");
        while (true) {
            std::string nextRuleSetName;
            for (Rule const &rule: ruleSet.rules) {
                bool isAccepted = false;
                if (rule.lessOrGreater == LESS) {
                    isAccepted = isAccepted || (part.get(rule.cat) < rule.threshold);
                } else if (rule.lessOrGreater == GREATER) {
                    isAccepted = isAccepted || (part.get(rule.cat) > rule.threshold);
                }
                if (isAccepted) {
                    if (rule.dst == ACCEPTED) {
                        return true;
                    } else if (rule.dst == REJECTED) {
                        return false;
                    } else {
                        nextRuleSetName = rule.dst;
                        break;
                    }
                }
            }
            ruleSet = ruleSets.at(nextRuleSetName);
        }

        assert(false);
    };

    int64 result = 0;
    for (auto const& part : parts) {

        if (isAccepted(part)) {
            result += part.sum();
        }
    }

    std::cout << result << std::endl;

    return 0;
}
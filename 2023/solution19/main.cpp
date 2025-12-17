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
static const int MIN_VALUE = 1;
static const int MAX_VALUE = 4000;
struct Rule {
    PartCategory cat;
    char lessOrGreater;
    int threshold;
    std::string dst;
    bool strict = true;

    [[nodiscard]] bool isUnconditional() const {
        if (threshold != MIN_VALUE)
            return false;

        assert(cat == 'x' && lessOrGreater == GREATER && !strict);
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

        if (ruleString.find(':') == std::string::npos) {
            rule.cat = 'x';
            rule.lessOrGreater = GREATER;
            rule.threshold = MIN_VALUE;
            rule.dst = ruleString;
            rule.strict = false;
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

Rule reverseRule(Rule rule) {
    rule.lessOrGreater = (rule.lessOrGreater == LESS ? GREATER : LESS);
    rule.strict = !rule.strict;
    rule.dst = "";
    return rule;
}

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
            if (rule.isUnconditional()) break;
        }

        return is;
    }
};

int main() {
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
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

    std::vector<RuleSet> acceptedRuleSets;
    std::queue<std::tuple<std::string, RuleSet>> q;
    q.emplace("in", RuleSet{});
    while (!q.empty()) {
        auto [ruleSetName, currentRuleSet] = q.front();
        q.pop();
        for (const auto& rule : ruleSets.at(ruleSetName).rules) {
            if (rule.dst == ACCEPTED) {
                auto ruleSet = currentRuleSet;
                ruleSet.rules.push_back(rule);
                acceptedRuleSets.push_back(ruleSet);
                currentRuleSet.rules.push_back(reverseRule(rule));
            } else if (rule.dst == REJECTED) {
                currentRuleSet.rules.push_back(reverseRule(rule));
            } else {
                assert(!rule.dst.empty());
                auto ruleSet = currentRuleSet;
                ruleSet.rules.push_back(rule);
                q.emplace(rule.dst, ruleSet);
                currentRuleSet.rules.push_back(reverseRule(rule));
            }
        }
    }

    const auto printRuleSets = [](const std::vector<RuleSet> ruleSets, bool skipUnconditional = true) {
        for (const auto& ruleSet : ruleSets) {
            for (const auto& rule : ruleSet.rules) {
                if (skipUnconditional && rule.isUnconditional()) {
                    continue;
                }
                std::cout << rule.cat << " " << rule.lessOrGreater << (rule.strict ? "" : "=") << " " << rule.threshold << ", ";
            }
            std::cout << std::endl;
        }
    };

//    printRuleSets(acceptedRuleSets);

    int64 result = 0;

    using AcceptedField = std::map<PartCategory, std::vector<bool>>;

    const auto acceptedFieldCount = [](const AcceptedField& acceptedField) -> int64 {
        int64 currentResult = 1;
        for (const auto& [cat, field] : acceptedField) {
            currentResult *= std::count(field.begin(), field.end(), true);
        }
        return currentResult;
    };

    const std::vector<PartCategory> AllCategories = {'x', 'm', 'a', 's'};

    std::vector<AcceptedField> acceptedFields;
    for (const auto& ruleSet : acceptedRuleSets) {
        AcceptedField acceptedField;
        for (char c : AllCategories) {
            auto& catField = acceptedField[c];
            catField.assign(MAX_VALUE - MIN_VALUE + 1, true);
        }

        for (const auto& rule : ruleSet.rules) {
            auto& catField = acceptedField.at(rule.cat);
            if (rule.lessOrGreater == LESS) {
                std::fill(catField.begin() + rule.threshold + 1 - MIN_VALUE, catField.end(), false);
            } else if (rule.lessOrGreater == GREATER) {
                std::fill(catField.begin(), catField.begin() + rule.threshold - MIN_VALUE, false);
            } else {
                assert(false);
            }

            if (rule.strict) {
                catField.at(rule.threshold - MIN_VALUE) = false;
            }
        }

        result += acceptedFieldCount(acceptedField);
        acceptedFields.push_back(acceptedField);
    }

    std::cout << result << std::endl;

    return 0;
}
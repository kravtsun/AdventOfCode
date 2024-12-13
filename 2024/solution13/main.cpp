#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <algorithm>

#include <cassert>
#include <cstdint>

// (X,Y)
using pii = std::pair<int64_t, int64_t>;

template<typename Token>
static void readToken(std::istream &is, const std::vector<Token> &expectedTokens) {
    Token token;
    is >> token;
    auto it = std::find(expectedTokens.begin(), expectedTokens.end(), token);
    if (it == expectedTokens.end()) {
        std::ostringstream os;
        os << "Read: " << token << ", Expected: ";
        for (const auto& t : expectedTokens) {
            os << t << ",";
        }
        throw std::runtime_error(os.str());
    }
}

static pii readButton(const std::string &line) {
    std::istringstream is{line};
    readToken<std::string>(is, {"Button"});
    readToken<std::string>(is, {"A:", "B:"});
    readToken<char>(is, {'X'});
    readToken<char>(is, {'+'});

    pii result;
    is >> result.first;
    readToken<char>(is, {','});
    readToken<char>(is, {'Y'});
    readToken<char>(is, {'+'});
    is >> result.second;
    return result;
}

static pii readPrize(const std::string &line) {
    std::istringstream is{line};
    readToken<std::string>(is, {"Prize:"});
    readToken<char>(is, {'X'});
    readToken<char>(is, {'='});

    pii result;
    is >> result.first;
    readToken<char>(is, {','});
    readToken<char>(is, {'Y'});
    readToken<char>(is, {'='});
    is >> result.second;
    return result;
}

using Sample = std::tuple<pii, pii, pii>;

static auto readSamples(const std::string &filepath, bool isStar2) {
    std::ifstream fin{filepath};

    std::vector<Sample> results;

    std::string line;
    while (std::getline(fin, line)) {
        while (line.empty() && std::getline(fin, line));
        if (line.empty()) break;

        auto a = readButton(line);
        std::getline(fin, line);
        auto b = readButton(line);
        std::getline(fin, line);
        auto prize = readPrize(line);

        if (isStar2) {
            const auto STAR2_BIAS = 10000000000000;
            prize.first += STAR2_BIAS;
            prize.second += STAR2_BIAS;
        }

        results.emplace_back(a, b, prize);
    }
    return results;
}

static auto star1(const std::vector<Sample> &samples) {
    const auto MAXCOST = std::numeric_limits<int64_t>::max();
    const int NMAX = 100;
    int64_t result = 0;
    for (auto [a, b, prize]: samples) {
        int64_t bestCost = MAXCOST;
        for (int i = 0; i <= NMAX; ++i) {
            for (int j = 0; j <= NMAX; ++j) {
                pii p{a.first * i + b.first * j, a.second * i + b.second * j};
                if (p == prize) {
                    int64_t cost = 3 * i + j;
                    bestCost = std::min(bestCost, cost);
                }
            }
        }
        if (bestCost != MAXCOST) {
            result += bestCost;
        }
    }
    return result;
}

template<typename T>
static T gcd(T a, T b) {
    return b ? gcd(b, a % b) : a;
}

template<typename T>
static T gcd(T a, T b, T &ia, T &ib) {
    if (b == 0) {
        ia = 1, ib = 0;
        return a;
    } else {
        T x, y;
        auto g = gcd(b, a % b, x, y);
        // b * x + (a % b) * y = g
        // b * x + (a - a//b * b) * y = g
        ia = y;
        ib = x - (a / b) * y;
        return g;
    }
}

template<typename T>
static std::tuple<T, T, T, T> solveDiophant(T a, T b, T c) {
    T ia, ib;
    auto g = gcd(a, b, ia, ib);
    if (c % g != 0) {
        return std::make_tuple(0, 0, 0, 0);
    }
    auto multiplier = c / g;
    ia *= multiplier;
    ib *= multiplier;

    auto l = a / g * b; // lcd

    // deltas: (ia + dia * k) * a + (ib - dib * k) * b = prize
    auto dia = l / a;
    auto dib = l / b;
    return std::make_tuple(ia, ib, dia, dib);
}

static auto star2(pii a, pii b, pii prize) {
    auto [iaFirst, ibFirst, diaFirst, dibFirst] = solveDiophant(a.first, b.first, prize.first);
    if (iaFirst == 0 && ibFirst == 0) {
        return 0LL;
    }

    // deltas: (iaFirst + diaFirst * kFirst) * a.first + (ibFirst - dibFirst * kFirst) * b.first = prize.first

    // Substitution:
    // (iaFirst + diaFirst * kFirst) * a.second + (ibFirst - dibFirst * kFirst) * b.second = prize.second

    // k - kFirst
    auto kNominator = prize.second - iaFirst * a.second - ibFirst * b.second;
    auto kDenominator = diaFirst * a.second - dibFirst * b.second;

    auto kg = gcd(abs(kNominator), abs(kDenominator));
    kNominator /= kg;
    kDenominator /= kg;
    if (kNominator % kDenominator != 0) {
        return 0LL;
    }

    auto k0 = kNominator / kDenominator;
    auto ia0 = iaFirst + k0 * diaFirst;
    auto ib0 = ibFirst - k0 * dibFirst;

    if (ia0 < 0 || ib0 < 0) {
        return 0LL;
    }

    return 3 * ia0 + ib0;
}

static auto star2(const std::vector<Sample> &samples) {
    int64_t result = 0;
    for (auto [a, b, prize]: samples) {
        result += star2(a, b, prize);
    }
    return result;
}

int main() {
    const auto inputFiles = {"example_input.txt", "input.txt"};
    std::cout << "STAR1: " << std::endl;
    for (const auto &filepath : inputFiles) {
        auto samples = readSamples(filepath, false);
        std::cout << "\t" << star1(samples) << std::endl;
    }

    std::cout << "STAR2: " << std::endl;
    for (const auto &filepath : inputFiles) {
        auto samples = readSamples(filepath, true);
        std::cout << "\t" << star2(samples) << std::endl;
    }

    return 0;
}

Alternative Solutions in Github Copilot: Alt+[, Alt+]

IMPORTANT!! TODO put this string everywhere:
// This software was partially written using Suggestions from GitHub Copilot.

```cpp type declarations
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vint64 = std::vector<int64>;
using pii = std::pair<int, int>;
```

```cpp memoization
std::map<KeyType, ResultType> memo;
auto memoIt = memo.find(key);
if (memoIt != memo.end()) {
    return memoIt->second;
}
```


```cpp from_string
using int64 = long long;
template<typename T = int64>
T from_string(const std::string &s) {
    std::istringstream is(s);
    T x;
    is >> x;
    return x;
}
```


template<typename T>
T gcd(T a, T b) {
    return b ? gcd(b, a%b) : a;   
}

 

```cpp BFS
struct Vertex {
    Vertex() {};
};

std::vector<std::vector<int>> g;

std::vector<bool> used(n, false);
std::queue<Vertex> q;
q.push(v);
used[v] = true;

while (!q.empty()) {
    auto v = q.front();
    q.pop();
    for (auto to : g[v].tos) {
        if (!used[to]) {
            q.push(to);
            used[to] = true;
        }
    }
}
```

```cpp read lines
static auto readLines(const std::string &filepath) {
    std::ifstream fin{filepath};
    assert(fin.is_open());
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}
```

```cpp handling points
using Point = std::pair<int, int>;

static auto addPoint(const Point &lhs, const Point &rhs) {
    return Point{lhs.first + rhs.first, lhs.second + rhs.second};
}

static bool isGoodPoint(const Point &p, int n, int m) {
    return p.first >= 0 && p.first < n && p.second >= 0 && p.second < m;
}

```

```cpp
static int char2digit(char c) {
    if (isalpha(c)) {
        assert(tolower(c) == c || toupper(c) == c);
        if (tolower(c) == c) {
            return 10 + c - 'a';
        } else {
            return 10 + c - 'A';
        }
    }
    return c - '0';
}
```

```cpp
#include <bits/stdc++.h>

static bool isPalindrome(int num) {
    std::vector<bool> binaryRepr;
    while (num) {
        binaryRepr.push_back(num & 1);
        num >>= 1;
    }

    std::reverse(binaryRepr.begin(), binaryRepr.end());
    for (int i = 0; i < binaryRepr.size() / 2; ++i) {
        if (binaryRepr[i] != binaryRepr[binaryRepr.size() - 1 - i]) return false;
    }
    return true;
}

using namespace std;

// T = int or int64
template<typename T = int>
struct EratosphenSieve {
    EratosphenSieve(const T MAXN)
        : MAXN(MAXN)
    {
        T maxI;
        for (maxI = 2; maxI * maxI <= MAXN; ++maxI);
        maxI--;

        isPrimeVector.assign(maxI + 1, true);
        isPrimeVector[0] = false;
        isPrimeVector[1] = false;

        for (T i = 2; i <= maxI; ++i) {
            for (T j = i * i; j <= maxI; j += i) {
                isPrimeVector[j] = false;
            }
        }

        for (T i = 2; i < isPrimeVector.size(); ++i) {
            if (isPrimeVector[i]) {
                primes.push_back(i);
            }
        }
    }

    auto getPrimes() const { return primes; }

    bool isPrime(T num) const {
        if (num < isPrimeVector.size()) {
            return isPrimeVector[num];
        }
        for (auto p : primes) {
            if (num % p == 0) {
                return false;
            }
        }
        return true;
    }

private:
    vector<bool> isPrimeVector;
    vector<int> primes;
    const int MAXN;
};

// 1-based index tree, but use 0-based indexes as input to functions
// (or correct indexes correspondingly, removing first increment in the loop initialization).
struct FenwickTree {
    vector<int> BIT;  // binary indexed tree
    int n;

    FenwickTree(int n)
            : n(n)
            , BIT(n + 1, 0)
    {}

    int sum(int idx) const {
        int res = 0;
        for (++idx; idx > 0; idx -= idx & -idx)
            res += BIT[idx];
        return res;
    }

    int range_sum(int l, int r) const {
        return sum(r) - sum(l - 1);
    }

    void add(int idx, int delta) {
        for (++idx; idx <= n; idx += idx & -idx)
            BIT[idx] += delta;
    }
};

template<typename T>
static void print(const std::vector<T> &v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
    std::cout << std::endl;
}

using int64 = long long;

/// Count inversions and sort the array
/// \tparam Iterator
/// \param begin
/// \param end
/// \return count of inversions
template<typename Iterator>
static int64 countInversions(Iterator begin, Iterator end) {
    const auto n = std::distance(begin, end);
    if (n <= 1) return 0;

    const auto n2 = n / 2;
    std::vector<typename Iterator::value_type> left(begin, begin + n2);
    auto leftBegin = left.begin();
    auto leftEnd = left.end();
    auto leftCnt = countInversions(leftBegin, leftEnd);


    std::vector<typename Iterator::value_type> right(begin + n2, end);
    auto rightBegin = right.begin();
    auto rightEnd = right.end();
    auto rightCnt = countInversions(rightBegin, rightEnd);

    auto res = leftCnt + rightCnt;

    for (auto itLeft = leftBegin,
                 itRight = rightBegin,
                 itNums = begin;
         itLeft != leftEnd || itRight != rightEnd;
            ) {
        if (itLeft == leftEnd || (itRight != rightEnd && *itLeft > *itRight)) {
            res += std::distance(itLeft, leftEnd);
            *itNums++ = *itRight++;
        } else {
            *itNums++ = *itLeft++;
        }
    }
    return res;
}

int64 countInversions(const vector<int> &nums) {
    auto numsCopy = nums;
    return countInversions(numsCopy.begin(), numsCopy.end());
}


template<typename T>
T sqr(T x) { return x * x; }

//    std::ifstream fin{"input.txt"};
//    std::ifstream fin{"example_input.txt"};
//    assert(fin.is_open());
//    std::cin.rdbuf(fin.rdbuf());


```


TODO 
formatting improvements
- remove all one-character variables, except for loop
- remove all assert instructions, replace with std::runtime_error
- remove `using int64 = long long`, use `std::int64_t` instead
- rename readLines into read_lines
- using namespace std
- all variabels in camelCase

TODO: 
get field with:
+ faster indexing
+ faster initializing left, right, up, down points
- faster searching for a specific symbol in the line - returning location point
- isGoodPoint should not have any parameters, other than the point. No field sizes. 
- print field fast


barebones for BFS, with parameters such as: 
- initializer lambda, 
- lambda returning the points to go into
- lambda on adding new point to the 

#define A auto
 
TODO
ensure that all header files are declared (maybe with modules?)

TODO remove `#include <bits/stdc++.h>`

TODO do not use int, where size_t or int32_t/int64_t is meant.

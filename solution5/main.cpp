#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>

using vint = std::vector<int>;
using int64 = uint64_t;
using vint64 = std::vector<int64>;

struct Seed {
    int64 src;
    int64 length;
//    Seed(int64 src, int64 length)
//        : src(src), length(length)
//    {}
    Seed() = default;
};

using SeedVector = std::vector<Seed>;

struct SeedSet {
    SeedVector seeds;
    explicit SeedSet(SeedVector seeds)
        : seeds(std::move(seeds))
    {
        std::sort(this->seeds.begin(), this->seeds.end(), [](const Seed& lhs, const Seed& rhs) {
            return lhs.src < rhs.src;
        });
    }

    [[nodiscard]] bool empty() const {
        return seeds.empty();
    }
};

static SeedSet readSeeds()
{
    std::string headerLine, line;
    std::getline(std::cin, headerLine);
    std::getline(std::cin, line); // empty line

    std::istringstream headerStream{headerLine};
    headerStream >> line; // seeds:
    SeedVector seeds;
    Seed seed{};
    while (headerStream >> seed.src >> seed.length) {
        seeds.push_back(seed);
    }
    return SeedSet{seeds};
}

struct Mapping {
    int64 src;
    int64 dst;
    int64 length;
    Mapping() = default;
};

struct SectionMap {
    std::vector<Mapping> mappings;

    SectionMap(std::vector<Mapping> mappings)
        : mappings(mappings)
    {
        std::sort(this->mappings.begin(), this->mappings.end(), [](const Mapping& lhs, const Mapping& rhs) {
            return lhs.src < rhs.src;
        });
    }

    [[nodiscard]] SeedSet getMapping(const SeedSet& srcSeedSet) const
    {
        SeedVector dstSeeds;
        for (auto srcSeed : srcSeedSet.seeds) {
            for (const auto& mapping : mappings) {
                if (srcSeed.length == 0) break;
                if (mapping.src <= srcSeed.src && srcSeed.src < mapping.src + mapping.length) {
                    Seed dstSeed;
                    dstSeed.src = mapping.dst + srcSeed.src - mapping.src;
                    if (mapping.src + mapping.length >= srcSeed.src + srcSeed.length) { // fully covered
                        dstSeed.length = srcSeed.length;
                        srcSeed.length = 0;
                    } else {
                        dstSeed.length = mapping.src + mapping.length - srcSeed.src;
                        srcSeed.length -= dstSeed.length;
                        srcSeed.src += dstSeed.length;
                    }
                    dstSeeds.push_back(dstSeed);
                }
            }

            if (srcSeed.length != 0) {
                dstSeeds.push_back(srcSeed);
            }
        }

        return SeedSet{dstSeeds};
    }

    [[nodiscard]] bool empty() const {
        return mappings.empty();
    }
};

static SectionMap readSection()
{
    std::string headerLine;
    std::getline(std::cin, headerLine);
    if (headerLine.empty())
        return SectionMap{{}};

    std::vector<Mapping> mappings;
    for (std::string line; std::getline(std::cin, line) && !line.empty(); )
    {
        std::istringstream lineStream{line};
        mappings.emplace_back();
        Mapping& mapping = mappings.back();
        lineStream >> mapping.dst >> mapping.src >> mapping.length;
    }
    return SectionMap{mappings};
}



int main() {
    FILE *f = freopen(WORKDIR "input.txt", "r", stdin);

    auto seedSet = readSeeds();

    while (true) {
        const auto sectionMap = readSection();
        if (sectionMap.empty()) break;
        seedSet = sectionMap.getMapping(seedSet);
        assert(!seedSet.empty());
    }

    int64 result = UINT64_MAX;
    for (auto seed : seedSet.seeds) {
        result = std::min(seed.src, result);
    }
    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

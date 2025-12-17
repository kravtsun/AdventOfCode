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


using int64 = uint64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

vs readField() {
    std::string line;
    vs field;
    while (std::getline(std::cin, line)) {
        field.push_back(line);
    }
    return field;
}

void printField(const vs& field) {
    for (const auto&f : field) {
        std::cout << f << std::endl;
    }
    std::cout << std::endl;
}

struct Beam {
    pii point; // (i, j)
    pii speed; // speed i, speed j

    Beam(pii point, pii speed)
            : point(point), speed(speed)
    {}
};

static const pii speedLeft{0, -1};
static const pii speedRight{0, 1};
static const pii speedUp{-1, 0};
static const pii speedDown{+1, 0};

int energizedTiles(const vs& field, const Beam& startBeam)
{
    const int height = field.size();
    const int width = field[0].size();
    vvint used(height, vint(width, 0));


    const auto move = [](const pii& p, const pii& speed) {
        return pii{p.first + speed.first, p.second + speed.second};
    };

    std::queue<Beam> q;
    std::map<pii, std::vector<pii>> passedBeamSpeeds;
    q.push(startBeam);

    while (!q.empty()) {
        const Beam beam = q.front();
        q.pop();

        const auto p = beam.point;
        const auto speed = beam.speed;
        if (p.first < 0 || p.first >= height || p.second < 0 || p.second >= width) continue;

        auto& pointPassedBeamSpeeds = passedBeamSpeeds[beam.point];
        if (std::find(pointPassedBeamSpeeds.begin(), pointPassedBeamSpeeds.end(), beam.speed) != pointPassedBeamSpeeds.end()) {
            continue;
        }
        pointPassedBeamSpeeds.push_back(beam.speed);
        used[p.first][p.second] = 1;

        const auto c = field[p.first][p.second];
        pii newSpeed;

        switch (c) {
            case '.':
                q.emplace(move(p, speed), speed);
                break;
            case '\\':

                if (speed == speedUp) {
                    newSpeed = speedLeft;
                } else if (speed == speedDown) {
                    newSpeed = speedRight;
                } else if (speed == speedLeft) {
                    newSpeed = speedUp;
                } else if (speed == speedRight) {
                    newSpeed = speedDown;
                } else {
                    assert(false);
                }
                q.emplace(move(p, newSpeed), newSpeed);
                break;
            case '/':
                if (speed == speedUp) {
                    newSpeed = speedRight;
                } else if (speed == speedDown) {
                    newSpeed = speedLeft;
                } else if (speed == speedLeft) {
                    newSpeed = speedDown;
                } else if (speed == speedRight) {
                    newSpeed = speedUp;
                } else {
                    assert(false);
                }
                q.emplace(move(p, newSpeed), newSpeed);
                break;
            case '|':
                newSpeed = speedUp;
                q.emplace(move(p, newSpeed), newSpeed);
                newSpeed = speedDown;
                q.emplace(move(p, newSpeed), newSpeed);
                break;
            case '-':
                newSpeed = speedLeft;
                q.emplace(move(p, newSpeed), newSpeed);
                newSpeed = speedRight;
                q.emplace(move(p, newSpeed), newSpeed);
                break;
            default:
                assert(false);
        }
    }

    int result = 0;
    for (auto uline : used) {
        result += std::count(uline.begin(), uline.end(), 1);
    }
    return result;
}

int main() {
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    auto field = readField();

    int result = -1;

    for (int i = 0; i < field.size(); ++i) {
        result = std::max(result, energizedTiles(field, Beam{pii{i, field[0].size() - 1}, speedLeft}));
        result = std::max(result, energizedTiles(field, Beam{pii{i, 0}, speedRight}));
    }

    for (int j = 0; j < field[0].size(); ++j) {
        result = std::max(result, energizedTiles(field, Beam{pii{0, j}, speedDown}));
        result = std::max(result, energizedTiles(field, Beam{pii{field.size() - 1, j}, speedUp}));
    }

    std::cout << result << std::endl;

    return 0;
}
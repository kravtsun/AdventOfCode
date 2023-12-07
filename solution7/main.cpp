#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>

using int64 = uint64_t;
const char JOKER = 'J';

int orderByCard(char card) {
    // the lesser the better.
    static const std::string cardOrder = "AKQT98765432J";
    const auto pos = cardOrder.find(card);
    assert(pos != std::string::npos);
    return static_cast<int>(pos);
};


struct HandAndBid {
    std::string hand;
    int winType; // the lesser the better.
    int bid;

    HandAndBid(std::string hand, int bid)
            : hand(std::move(hand)), bid(bid) {
        this->winType = getWinType(this->hand);
    }

    static int getWinType(const std::string &hand) {
        // 0 Five of a kind, where all five cards have the same label: AAAAA
        // 1 Four of a kind, where four cards have the same label and one hand has a different label: AA8AA
        // 2 Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
        // 3 Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other hand in the hand: TTT98
        // 4 Two pair, where two cards share one label, two other cards share a second label, and the remaining hand has a third label: 23432
        // 5 One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
        // 6 High hand, where all cards' labels are distinct: 23456

        std::map<char, int> cardCount;
        for (char c: hand) {
            cardCount[c]++;
        }

        char mostFrequentCard = '2';
        int mostFrequentCount = -1;
        for (auto [card, cnt] : cardCount) {
            if (card != JOKER && cnt > mostFrequentCount) {
                mostFrequentCount = cnt;
                mostFrequentCard = card;
            }
        }

        cardCount[mostFrequentCard] += cardCount[JOKER];
        cardCount.erase(JOKER);

        if (cardCount.size() == 1) {
            assert(cardCount.begin()->second == 5);
            return 0;
        }

        int pairs = 0;
        bool hasTriple = false, hasQuadruple = false;
        for (auto [_, cnt]: cardCount) {
            if (cnt == 2) {
                pairs++;
            } else if (cnt == 3) {
                hasTriple = true;
            } else if (cnt == 4) {
                hasQuadruple = true;
            }
        }
        if (hasQuadruple) {
            return 1;
        }

        if (hasTriple) {
            if (pairs) {
                return 2;
            } else {
                return 3;
            }
        }

        if (pairs) {
            if (pairs == 2) {
                return 4;
            } else {
                return 5;
            }
        }

        return 6;
    }

    // First come the least valuable.
    friend bool operator<(const HandAndBid &lhs, const HandAndBid &rhs) {
        if (lhs.winType != rhs.winType) {
            return lhs.winType > rhs.winType;
        }
        assert(lhs.hand.size() == rhs.hand.size());
        for (int i = 0; i < lhs.hand.size(); ++i) {
            if (lhs.hand[i] != rhs.hand[i]) {
                return orderByCard(lhs.hand[i]) > orderByCard(rhs.hand[i]);
            }
        }
        return false;
    }
};

int main() {
    FILE *f = freopen("input.txt", "r", stdin);

    std::vector<HandAndBid> handAndBids;
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};

        std::string hand;
        int bid;
        lineStream >> hand >> bid;
        handAndBids.emplace_back(hand, bid);
    }

    std::sort(handAndBids.begin(), handAndBids.end());

    int64 result = 0;
    for (int i = 0; i < handAndBids.size(); ++i) {
        result += handAndBids[i].bid * (i + 1);
    }

    std::cout << result << std::endl;

    fclose(f);
    return 0;
}

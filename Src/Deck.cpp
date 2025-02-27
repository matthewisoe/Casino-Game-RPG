#include "Deck.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>

Deck::Deck() {
    std::vector<std::string> suits = {"hearts", "diamonds", "clubs", "spades"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king", "ace"};
    std::vector<int> values = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

    for (const auto& suit : suits) {
        for (size_t i = 0; i < ranks.size(); ++i) {
            cards.emplace_back(suit, ranks[i], values[i]);
        }
    }
    Shuffle();
}

void Deck::Shuffle() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(cards.begin(), cards.end(), std::mt19937(std::random_device()()));
}

Card Deck::DrawCard() {
    if (cards.empty()) return Card("", "", 0);
    Card drawnCard = cards.back();
    cards.pop_back();
    return drawnCard;
}

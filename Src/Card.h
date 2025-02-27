#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
    std::string suit;
    std::string rank;
    int value;

    Card(const std::string& s, const std::string& r, int v);
};

#endif // CARD_H

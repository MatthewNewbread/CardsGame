#ifndef CARD_H
#define CARD_H
#include <iostream>

class Card
{
public:
    enum class Rank {two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, none};
    enum class Suit {clubs, diamonds, hearths, spades, none};

    Card(Rank m_rank, Suit m_suit);
    Suit get_suit() const;
    Rank get_rank() const;
    int get_value() const;
    std::string get_string_card() const;
    bool operator==(const Card& card) const;
    bool operator>(const Card& card) const;
    bool operator<(const Card& card) const;
private:
    Rank rank;
    Suit suit;
};

#endif // CARD_H

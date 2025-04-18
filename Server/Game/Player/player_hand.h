#ifndef PLAYER_HAND_H
#define PLAYER_HAND_H
#include <vector>
#include <functional>
#include <map>
#include "Game/Deck/card.h"

class Hand
{
public:
    Hand();
    std::vector<Card> get_cards() const;
    int get_score() const;
    void set_score(int value);
    void add_card(Card card);
    void clear_hand();
private:
    std::vector<Card> cards;
    int score;
};

#endif // PLAYER_HAND_H

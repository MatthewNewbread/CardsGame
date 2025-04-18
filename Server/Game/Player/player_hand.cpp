#include "player_hand.h"

Hand::Hand(): score{0}
{
}

std::vector<Card> Hand::get_cards() const
{
    return cards;
}

int Hand::get_score() const
{
    return score;
}

void Hand::set_score(int value)
{
    score = value;
}

void Hand::add_card(Card card)
{
    cards.push_back(card);
}

void Hand::clear_hand()
{
    cards.clear();
    score = 0;
}

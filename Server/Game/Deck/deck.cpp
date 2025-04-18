#include "deck.h"

int Deck::card_index = 0;

Deck::Deck()
{}

void Deck::refresh()
{
    deck.clear();
    for (int counter_rang = 0; counter_rang <= static_cast<int>(Card::Rank::ace); counter_rang++)
    {
        for (int counter_suit = 0; counter_suit <= static_cast<int>(Card::Suit::spades); counter_suit++)
        {
            deck.push_back({static_cast<Card::Rank>(counter_rang), static_cast<Card::Suit>(counter_suit)});
        }
    }
}

void Deck::shuffle()
{
    QRandomGenerator rng;
    for (int i = deck.size() - 1; i > 0; --i)
    {
        int j = rng.bounded(0, i);
        std::swap(deck[i], deck[j]);
    }
    card_index = 0;
}

void Deck::cutoff()
{
    QRandomGenerator rng;
    deck.erase(deck.begin(), deck.begin() + rng.bounded(0, (size() == 52)? 15: 10));
}

void Deck::preparation()
{
    refresh();
    shuffle();
    cutoff();
}

const Card& Deck::deal_card()
{
    return deck[card_index++];
}

int Deck::size() const
{
    return deck.size();
}

#include "card.h"

Card::Card(Rank m_rank, Suit m_suit): rank{m_rank}, suit{m_suit}
{}

Card::Rank Card::get_rank() const
{
    return rank;
}

Card::Suit Card::get_suit() const
{
    return suit;
}

int Card::get_value() const
{
    return static_cast<int>(rank) + 2;
}

std::string Card::get_string_card() const
{
    static const std::string rank_names[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "N"};
    static const std::string suit_names[] = {"H", "C", "D", "S", "N"};
    return rank_names[static_cast<int>(rank)] + suit_names[static_cast<int>(suit)];
}

bool Card::operator==(const Card& card) const
{
    return get_rank() == card.get_rank();
}

bool Card::operator>(const Card &card) const
{
    return get_rank() > card.get_rank();
}

bool Card::operator<(const Card &card) const
{
    return get_rank() < card.get_rank();
}

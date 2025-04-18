#include "player_bank.h"

Bank::Bank(int chips): chips{chips}, bet{0}
{}

int Bank::get_chips() const
{
    return chips;
}

void Bank::set_chips(int new_chips)
{
    chips = new_chips;
}

int Bank::get_bet() const
{
    return bet;
}

void Bank::set_bet(int new_bet)
{
    bet = new_bet;
}

void Bank::update_chips(int new_chips)
{
    chips += new_chips;
}

void Bank::update_bet(int new_bet)
{
    bet += new_bet;
}

void Bank::take_bet(int bet_amount)
{
    chips -= bet_amount - bet;
    bet = bet_amount;
}

bool Bank::is_zero_chips() const
{
    return (chips == 0)? true: false;
}

bool Bank::is_not_enough_chips(int buffer_bet) const
{
    return (buffer_bet > chips)? true: false;
}

void Bank::clear_bet()
{
    bet = 0;
}

void Bank::clear_chips()
{
    chips = 0;
}

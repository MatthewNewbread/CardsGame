#include "player_role_human.h"

Human::Human(): Player(Role::Human)
{
}

void Human::make_bet(int min_bet)
{
    std::string message = "\nEnter the bet(min bet - " + std::to_string(min_bet) + "): ";
    int bet = error.validate_input_int(message, min_bet, bank.get_chips());
    bank.set_bet(bet);
}

#include "player_role_bot.h"

Bot::Bot(): Player(Role::Bot)
{
}

void Bot::make_bet(int min_bet)
{
    bank.set_bet(rng.get_random_number(min_bet, bank.get_chips()));
}

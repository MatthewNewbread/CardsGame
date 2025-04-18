#ifndef PLAYER_BANK_H
#define PLAYER_BANK_H
#include <iostream>

class Bank
{
private:
    int chips;
    int bet;
public:
    Bank(int chips);
    int get_chips() const;
    int get_bet() const;
    void set_chips(int new_chips);
    void set_bet(int new_bet);
    void update_chips(int new_chips);
    void update_bet(int new_bet);
    void take_bet(int bet_amount);
    bool is_zero_chips() const;
    bool is_not_enough_chips(int buffer_bet) const;
    void clear_bet();
    void clear_chips();
};

#endif // PLAYER_BANK_H

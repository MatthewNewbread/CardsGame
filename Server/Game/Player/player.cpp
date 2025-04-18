#include "player.h"

Player::Player(const QString& name, Role role, int chips, QTcpSocket* socket)
    : name{name}, role{role}, state{State::Playing}, result{Results::Defualt}, active{Active::Active}, bank{chips}, socket{socket}
{}

QString Player::get_name() const
{
    return name;
}

Player::Active Player::get_active() const
{
    return active;
}

QTcpSocket* Player::get_socket() const
{
    return socket;
}

Player::Role Player::get_role() const
{
    return role;
}

Player::Results Player::get_results() const
{
    return result;
}

Player::State Player::get_state() const
{
    return state;
}

Bank Player::get_bank() const
{
    return bank;
}

Hand Player::get_hand() const
{
    return hand;
}

void Player::set_state(State new_state)
{
    state = new_state;
}

void Player::set_result(Results new_result)
{
    result = new_result;
}

void Player::set_active(Active new_active)
{
    active = new_active;
}

void Player::clear()
{
    bank.clear_bet();
    hand.clear_hand();
    state = State::Playing;
    result = Results::Defualt;
}

bool Player::has_socket() const
{
    return socket != nullptr;
}

bool Player::is_player_in_role(Role other_role) const
{
    return role == other_role;
}

bool Player::is_player_in_state(State other_state) const
{
    return state == other_state;
}

bool Player::is_player_in_result(Results other_result) const
{
    return result == other_result;
}

bool Player::is_player_in_active(Active other_active) const
{
    return active == other_active;
}


#ifndef PLAYER_H
#define PLAYER_H
#include <QTcpSocket>
#include <QString>
#include <memory>
#include "player_bank.h"
#include "player_hand.h"

class Player
{
public:
    enum class Role {Bot, Human, Dealer};
    enum class State {Playing, Waiting, Stopped};
    enum class Results {Winner, Loser, Defualt};
    enum class Active {Active, Disconnected};

    Player(const QString& name, Role role, int chips = 0, QTcpSocket* socket = nullptr);
    Bank get_bank() const;
    Hand get_hand() const;
    Role get_role() const;
    Results get_results() const;
    State get_state() const;
    QString get_name() const;
    Active get_active() const;
    QTcpSocket* get_socket() const;
    void set_state(State new_state);
    void set_result(Results new_result);
    void set_active(Active new_active);
    void clear();
    bool has_socket() const;
    bool is_player_in_role(Role other_role) const;
    bool is_player_in_state(State other_state) const;
    bool is_player_in_result(Results other_result) const;
    bool is_player_in_active(Active other_active) const;
private:
    QString name;
    Role role;
    State state;
    Results result;
    Active active;
    Bank bank;
    Hand hand;
    QTcpSocket* socket;
};

#endif // PLAYER_H

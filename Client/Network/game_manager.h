#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "client.h"

class Game;

class Game_manager: public QObject
{
    Q_OBJECT
public:
    explicit Game_manager(Client* client, QObject* parent = nullptr);
    void choose_action(const QString& action);
    void set_bet(int amount);
    void exit_player();
private:
    Client* client;
    void send_request(Client::Request_Type type, const QJsonObject& json_data);
};

#endif // GAME_MANAGER_H

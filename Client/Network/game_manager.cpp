#include "game_manager.h"

Game_manager::Game_manager(Client *client, QObject *parent): QObject(parent), client(client)
{}

void Game_manager::choose_action(const QString &action)
{
    QJsonObject json_data;
    json_data["action"] = action;
    send_request(Client::Request_Type::Player_Send_Action, json_data);
}

void Game_manager::set_bet(int amount)
{
    QJsonObject json_data;
    json_data["bet"] = amount;
    send_request(Client::Request_Type::Player_Send_Bet, json_data);
}

void Game_manager::exit_player()
{
    send_request(Client::Request_Type::Player_Exit, {});
}

void Game_manager::send_request(Client::Request_Type type, const QJsonObject& json_data)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(type);
    packet["data"] = json_data;
    QJsonDocument doc(packet);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    client->send_request(data);
}

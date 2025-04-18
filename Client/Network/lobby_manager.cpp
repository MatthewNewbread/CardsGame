#include "lobby_manager.h"

Lobby_manager::Lobby_manager(Client* client, QObject* parent): QObject(parent), client(client)
{}

void Lobby_manager::create_session(const QString &username, const QString &game, int max_players, int count_bots, int min_bet, int starting_chips)
{
    QJsonObject request;
    request["username"] = username;
    request["game"] = game;
    request["max_players"] = max_players;
    request["count_bots"] = count_bots;
    request["min_bet"] = min_bet;
    request["starting_chips"] = starting_chips;
    send_request(Client::Request_Type::Create_Session, request);
}

void Lobby_manager::join_session(const QString &username, int id)
{
    QJsonObject request;
    request["username"] = username;
    request["id"] = id;
    send_request(Client::Request_Type::Join_Session, request);
}

void Lobby_manager::view_sessions()
{
    send_request(Client::Request_Type::View_Sessions, {});
}

void Lobby_manager::get_settings()
{
    send_request(Client::Request_Type::Settings_Session, {});
}

void Lobby_manager::send_request(Client::Request_Type request_type, const QJsonObject& json_data)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["data"] = json_data;
    QJsonDocument doc(packet);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    client->send_request(data);
}

#include "lobby_manager.h"
#include "Game/game.h"

Lobby_manager::Lobby_manager(Server *server, QObject *parent): QObject(parent), server(server)
{}

void Lobby_manager::create_session(QTcpSocket* socket, const QString& username, const QString& game, int max_players, int count_bots, int min_bet, int starting_chips)
{
    QMutexLocker locker(&hub_mutex);
    lobby->create_game_session(socket, username, game, max_players, count_bots, min_bet, starting_chips);
    send_response(socket, Server::Request_Type::Create_Session, {});
}

void Lobby_manager::join_session(QTcpSocket* socket, const QString& username, int id)
{
    QMutexLocker locker(&hub_mutex);
    lobby->join_game_session(socket, username, id);
    send_response(socket, Server::Request_Type::Join_Session, {});
}

void Lobby_manager::view_sessions(QTcpSocket *socket)
{
    QMutexLocker locker(&hub_mutex);
    QJsonArray lobby_info = lobby->get_all_sessions_info();
    QJsonObject response;
    response["lobby"] = lobby_info;
    send_response(socket, Server::Request_Type::View_Sessions, response);
}

void Lobby_manager::session_settings(QTcpSocket *socket)
{
    QMutexLocker locker(&hub_mutex);
    send_response(socket, Server::Request_Type::Settings_Session, lobby->get_all_settings_info());
}

void Lobby_manager::send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["response"] = response;
    server->send_packet(socket, packet);
}


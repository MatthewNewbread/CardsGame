#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H
#include "server.h"
#include "lobby.h"

class Lobby_manager: public QObject
{
    Q_OBJECT
public:
    explicit Lobby_manager(Server* server, QObject* parent = nullptr);
    void create_session(QTcpSocket* socket, const QString& username, const QString& game, int max_players, int count_bots, int min_bet, int starting_chips);
    void join_session(QTcpSocket* socket, const QString& username, int id);
    void view_sessions(QTcpSocket* socket);
    void session_settings(QTcpSocket* socket);
private:
    Server* server;
    Lobby* lobby;
    QMutex hub_mutex;

    void send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response);
};

#endif // LOBBY_MANAGER_H

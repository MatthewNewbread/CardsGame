#ifndef LOBBY_MANAGER_H
#define LOBBY_MANAGER_H
#include "client.h"

class Lobby_manager: public QObject
{
public:
    explicit Lobby_manager(Client* client, QObject* parent = nullptr);
    void create_session(const QString& username, const QString& game, int max_players, int count_bots, int min_bet, int starting_chips);
    void join_session(const QString& username, int id);
    void view_sessions();
    void get_settings();
signals:
    void session_created();
    void session_joined();
    void sessions_viewed(const QJsonArray& sessions);
    void server_data_received(int max_seats, int max_chips, int min_bet, const QJsonArray& game_names);
private:
    Client* client;
    void send_request(Client::Request_Type request_type, const QJsonObject& json_data);
};

#endif // LOBBY_MANAGER_H

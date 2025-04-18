#ifndef LOBBY_H
#define LOBBY_H
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include "Game/game.h"

class Lobby
{
public:
    Lobby();
    void create_game_session(QTcpSocket* socket, const QString& username, const QString& game, int max_players, int count_bots, int min_bet, int starting_chips);
    bool join_game_session(QTcpSocket* socket, const QString& username, int id);
    void remove_game_session(int id);
    QJsonArray get_all_sessions_info();
    QJsonObject get_all_settings_info();
private:
    static const int MAX_PLAYERS = 8;
    static const int MIN_BET = 20;
    static const int MAX_CHIPS = 10000;
    static int id_session;
    QMap<int, Game*> sessions;
    QMap<QString, std::function<Game*(QString, int, int, int, int, std::vector<std::shared_ptr<Player>>&)>> games;

    QJsonObject get_all_session_info(int id);
};

#endif // LOBBY_H

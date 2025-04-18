#ifndef GAME_CONNECTION_H
#define GAME_CONNECTION_H
#include <Game/Player/player.h>

class Game_Connection
{
public:
    Game_Connection();
    std::vector<std::shared_ptr<Player>>& preparation_players(QTcpSocket* socket, const QString& username, int max_players, int count_bots, bool need_dealer);
    std::vector<std::shared_ptr<Player>>& updating_players(int max_players, int count_bots, int min_bet);
    void add_user(QTcpSocket* socket, const QString& username, int max_players);
    void disconnect_user(QTcpSocket* socket);
private:
    std::vector<std::shared_ptr<Player>> players;
    QMap<QString, QTcpSocket*> waiting_players;

    void add_bots(int bots);
    void add_dealer();
    void process_waiting_queue(int max_players);
    void restore_bots_if_needed(int count_bots);
};

#endif // GAME_CONNECTION_H

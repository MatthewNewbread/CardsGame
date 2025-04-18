#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include "server.h"
#include "Game/game.h"

class Game_manager: public QObject
{
    Q_OBJECT
public:
    explicit Game_manager(Game* game, Server* server, QObject* parent = nullptr);
    void process_player_action(QTcpSocket* socket, std::string action);
    void process_player_place_bet(QTcpSocket* socket, int bet);
    void process_player_exit(QTcpSocket* socket);
    void process_game_status();
public slots:
    void log_db_game_session(QString name, std::shared_ptr<Player>& player, int chips);
    void send_player_action(std::shared_ptr<Player>& player, const std::vector<std::string> available_actions);
    void send_player_place_bet(std::shared_ptr<Player>& player, int min_bet, int max_bet);
    void state_game();
    void state_players(const std::shared_ptr<Player>& active_player = nullptr);
private:
    Game* game;
    Server* server;
    DataBase* db;
    QMutex game_mutex;
    QMap<QString, std::function<QJsonObject(QJsonObject)>> state_games;

    std::shared_ptr<Player> get_player_by_socket(QTcpSocket* socket) const;
    void send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response);
    void send_all_client(Server::Request_Type request_type, const QJsonObject& response);
    QJsonArray serialize_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player);
};

#endif // GAME_MANAGER_H

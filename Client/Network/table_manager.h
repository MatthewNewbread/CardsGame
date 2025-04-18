#ifndef TABLE_MANAGER_H
#define TABLE_MANAGER_H
#include "client.h"

class Table_manager: public QObject
{
    Q_OBJECT
public:
    explicit Table_manager(Client* client, QObject* parent = nullptr);
    void start_game();
    void choose_game(int game);
    void choose_table_type(int table_type);
    void choose_bot_count(int bot_count);
    void choose_starting_chips(int chips);
    void request_table_info();
    void handle_info_table(int min_bet, int max_players, QStringList games_list, QStringList table_type_list);
signals:
    void table_info_received(int min_bet, int max_players, QStringList games_list, QStringList table_type_list);
private:
    Client* client;
    template<typename Data>
    void send_request(Client::Request_Type request_type, const Data& data);
};

#endif // TABLE_MANAGER_H

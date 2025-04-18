#ifndef TABLE_MANAGER_H
#define TABLE_MANAGER_H
#include "server.h"
#include "table.h"


class Table_manager: QObject
{
    Q_OBJECT
public:
    enum class Setting_Type
    {
        Game,
        Table_Type,
        Bot_Count,
        Starting_Chips
    };

    explicit Table_manager(Server* server, QObject *parent = nullptr);
    void process_start_game(QTcpSocket* socket);
    void process_choose_game(QTcpSocket* socket, Table::Games game);
    void process_choose_table_type(QTcpSocket* socket, Table::Type table_type);
    void process_choose_bot_count(QTcpSocket* socket, int bot_count);
    void process_choose_starting_chips(QTcpSocket* socket, int chips);

    int get_min_bet() const;
    int get_max_players() const;
    QStringList get_games_list() const;
    QStringList get_table_type_list() const;
private:
    Table table;
    Server* server;
};

#endif // TABLE_MANAGER_H

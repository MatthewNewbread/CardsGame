#include "table_manager.h"

Table_manager::Table_manager(Server* server, QObject *parent): QObject(parent), server(server)
{

}

void Table_manager::process_start_game(QTcpSocket *socket)
{
    table.start_game();
    server->send_packet(socket, QByteArray::fromRawData("Game started", 12));
}

void Table_manager::process_choose_game(QTcpSocket *socket, Table::Games game)
{
    table.set_game(table.get_game_by_enum(game));
    server->send_packet(socket, QByteArray::fromRawData("Game chosen", 11));
}

void Table_manager::process_choose_table_type(QTcpSocket *socket, Table::Type table_type)
{
    table.set_min_bet(table.get_min_bet_for_type(table_type));
    server->send_packet(socket, QByteArray::fromRawData("Table type chosen", 17));
}

void Table_manager::process_choose_bot_count(QTcpSocket *socket, int bot_count)
{
    table.set_count_bots(bot_count);
    server->send_packet(socket, QByteArray::fromRawData("Bot count chosen", 16));
}

void Table_manager::process_choose_starting_chips(QTcpSocket *socket, int chips)
{
    table.set_starting_chips(chips);
    server->send_packet(socket, QByteArray::fromRawData("Starting chips chosen", 21));
}

int Table_manager::get_min_bet() const
{
    return table.get_min_bet();
}

int Table_manager::get_max_players() const
{
    return Table::MAX_BOT;
}

QStringList Table_manager::get_games_list() const
{
    QStringList list;
    for (const auto& game: table.get_name_games())
    {
        list.append(QString::fromStdString(game.second));
    }
    return list;
}

QStringList Table_manager::get_table_type_list() const
{
    QStringList list;
    for (const auto& type: table.get_name_type_tables())
    {
        list.append(QString::fromStdString(type.second));
    }
    return list;
}

#include "table_manager.h"

Table_manager::Table_manager(Client *client, QObject *parent): QObject(parent), client(client)
{

}

void Table_manager::start_game()
{
    send_request(Client::Request_Type::Start_Game, 0);
}

void Table_manager::choose_game(int game)
{
    send_request(Client::Request_Type::Setting_Choose_Game, game);
}

void Table_manager::choose_table_type(int table_type)
{
    send_request(Client::Request_Type::Setting_Choose_Table_Type, table_type);
}

void Table_manager::choose_bot_count(int bot_count)
{
    send_request(Client::Request_Type::Setting_Choose_Bot_Count, bot_count);
}

void Table_manager::choose_starting_chips(int chips)
{
    send_request(Client::Request_Type::Setting_Choose_Starting_Chips, chips);
}

void Table_manager::request_table_info()
{
    send_request(Client::Request_Type::Table_Info, 0);
}

void Table_manager::handle_info_table(int min_bet, int max_players, QStringList games_list, QStringList table_type_list)
{
    emit table_info_received(min_bet, max_players, games_list, table_type_list);
}

template<typename Data>
void Table_manager::send_request(Client::Request_Type request_type, const Data &data)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << data;
    client->send_request(static_cast<quint8>(request_type), packet);
}

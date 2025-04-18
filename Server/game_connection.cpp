#include "game_connection.h"

Game_Connection::Game_Connection(){}

std::vector<std::shared_ptr<Player> > &Game_Connection::preparation_players(QTcpSocket *socket, const QString &username, int max_players, int count_bots, bool need_dealer)
{
    add_user(socket, username, max_players);
    add_bots(count_bots);
    if (need_dealer) add_dealer();
    return players;
}

void Game_Connection::add_user(QTcpSocket* socket, const QString& username, int max_players)
{
    if ((int)players.size() < max_players)
        players.push_back(std::make_shared<Player>(username, Player::Role::Human, socket));
    else
        waiting_players[username] = socket;
}

void Game_Connection::add_bots(int bots)
{
    for (int i = 0; i < bots; i++) players.push_back(std::make_shared<Player>("Bot_" + std::to_string(i), Player::Role::Bot));
}

void Game_Connection::add_dealer()
{
    players.push_back(std::make_shared<Player>("Dealer", Player::Role::Dealer));
}

std::vector<std::shared_ptr<Player>>& Game_Connection::updating_players(int max_players, int count_bots, int min_bet)
{
    players.erase(std::remove_if(players.begin(), players.end(), [min_bet](const std::shared_ptr<Player>& player) {return player->get_bank().get_chips() < min_bet;}), players.end());
    process_waiting_queue(max_players);
    restore_bots_if_needed(count_bots);
    return players;
}

void Game_Connection::disconnect_user(QTcpSocket *socket)
{
    auto it = std::remove_if(players.begin(), players.end(), [socket](const std::shared_ptr<Player>& player) {return player->get_socket() == socket;});
    if (it != players.end()) players.erase(it, players.end());
}

void Game_Connection::process_waiting_queue(int max_players)
{
    while ((int)players.size() < max_players && !waiting_players.isEmpty())
    {
        auto it = waiting_players.begin();
        players.push_back(std::make_shared<Player>(it.key(), Player::Role::Human, it.value()));
        waiting_players.erase(it);
    }
}

void Game_Connection::restore_bots_if_needed(int count_bots)
{
    int current_bots = std::count_if(players.begin(), players.end(), [](const std::shared_ptr<Player>& player) {return player->is_player_in_role(Player::Role::Bot);});
    if (waiting_players.isEmpty() && current_bots < count_bots) add_bots(current_bots);
}

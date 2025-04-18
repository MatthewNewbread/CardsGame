#include "lobby.h"
#include "Game/Game-Poker/game_poker.h"
#include "Game/Game-Blackjack/game_blackjack.h"

Lobby::Lobby()
{
    id_session = 0;
    games =
    {
        {"Poker", [](QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players){return new Poker(name, min_bet, max_players, max_bots, starting_chips, players);}},
        {"Blackjack", [](QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players){return new Blackjack(name, min_bet, max_players, max_bots, starting_chips, players);}}
    };
}

void Lobby::create_game_session(QTcpSocket *socket, const QString &username, const QString &name, int max_players, int max_bots, int min_bet, int starting_chips)
{
    std::vector<std::shared_ptr<Player>> players;
    Game* game = games.find(name).value()(name, min_bet, max_players, max_bots, starting_chips, players);
    if (game->get_is_dealer()) game->add_dealer();
    game->add_user(socket, username);
    game->add_bots(max_bots);
    sessions[id_session++] = game;
}

bool Lobby::join_game_session(QTcpSocket* socket, const QString& username, int id)
{
    Game* game = sessions[id];
    if (game->is_full()) return false;
    game->add_user(socket,username);
    return true;
}

void Lobby::remove_game_session(int id)
{
    sessions.remove(id);
}

QJsonArray Lobby::get_all_sessions_info()
{
    QJsonArray sessions_info;
    foreach(const int& id, sessions.keys()) sessions_info.append(get_all_session_info(id));
    return sessions_info;
}

QJsonObject Lobby::get_all_session_info(int id)
{
    QJsonObject session_info;
    Game* session = sessions[id];
    session_info["game_name"] = session->get_name();
    session_info["max_players"] = session->get_max_players();
    session_info["count_bots"] = session->get_max_bots();
    session_info["min_bet"] = session->get_min_bet();
    session_info["starting_chips"] = session->get_starting_chips();
    session_info["current_players"] = (int)session->get_players().size();
    session_info["id"] = id;
    return session_info;
}

QJsonObject Lobby::get_all_settings_info()
{
    QJsonObject settings_info;
    QJsonArray games_name;
    settings_info["max_players"] = MAX_PLAYERS;
    settings_info["min_bet"] = MIN_BET;
    settings_info["max_chips"] = MAX_CHIPS;
    foreach(const QString& name, games.keys()) games_name.append(name);
    settings_info["games_name"] = games_name;
    return settings_info;
}

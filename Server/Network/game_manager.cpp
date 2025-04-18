#include "game_manager.h"
#include "Game/Game-Blackjack/game_blackjack.h"
#include "Game/Game-Poker/game_poker.h"

Game_manager::Game_manager(Game *game, Server *server, QObject *parent): QObject(parent), game(game), server(server), db(new DataBase())
{
    state_games =
    {
        {"Blackjack", [](QJsonObject game_data)
         {
             game_data["type"] = "Blackjack";
             return game_data;
         }
        },
        {"Poker", [game](QJsonObject game_data)
         {
             game_data["type"] = "Poker";
             game_data["pot"] = game->get_pot();
             QJsonArray cards_array;
             for (const Card& card : dynamic_cast<Poker*>(game)->get_table_cards())
             {
                 QJsonObject card_obj;
                 card_obj["card"] = QString::fromStdString(card.get_string_card());
                 cards_array.append(card_obj);
             }
             game_data["cards"] = cards_array;
             return game_data;
         }
        }
    };
    connect(game, &Game::log_db_game_sessions, this, &Game_manager::log_db_game_session);
    connect(game, &Game::human_turn, this, &Game_manager::send_player_action);
    connect(game, &Game::human_place_bet, this, &Game_manager::send_player_place_bet);
    connect(game, &Game::game_state, this, &Game_manager::state_game);
}

void Game_manager::send_player_action(std::shared_ptr<Player>& player, const std::vector<std::string> available_actions)
{
    QMutexLocker locker(&game_mutex);
    QJsonObject response;
    QJsonArray actions_array;
    for (const std::string& action : available_actions) actions_array.append(QString::fromStdString(action));
    response["actions"] = actions_array;
    send_response(player->get_socket(), Server::Request_Type::Player_Send_Action, response);
}

void Game_manager::send_player_place_bet(std::shared_ptr<Player>& player, int min_bet, int max_bet)
{
    QMutexLocker locker(&game_mutex);
    QJsonObject response;
    response["min_bet"] = min_bet;
    response["max_bet"] = max_bet;
    send_response(player->get_socket(), Server::Request_Type::Player_Send_Bet, response);
}

void Game_manager::process_player_action(QTcpSocket *socket, std::string action)
{
    auto player = get_player_by_socket(socket);
    if (player) game->player_action(player, action);
}

void Game_manager::process_player_place_bet(QTcpSocket *socket, int bet)
{
    auto player = get_player_by_socket(socket);
    if (player) game->place_bet(player, bet);
}

void Game_manager::process_player_exit(QTcpSocket *socket)
{
    game->disconnect_user(socket);
}

void Game_manager::process_game_status()
{

}

std::shared_ptr<Player> Game_manager::get_player_by_socket(QTcpSocket *socket) const
{
    for (const auto& player: game->get_players()) if (player->get_socket() == socket) return player;
    return nullptr;
}

void Game_manager::state_game()
{
    QMutexLocker locker(&game_mutex);
    QJsonObject state;
    state_games.find(game->get_name()).value()(state);
    send_all_client(Server::Request_Type::Game_State, state);
}

// переделать в пользу all_players
void Game_manager::state_players(const std::shared_ptr<Player>& active_player)
{
    QMutexLocker locker(&game_mutex);
    QJsonArray players_array;
    std::vector<std::shared_ptr<Player>> all_players = game->get_players();
    all_players.insert(all_players.end(), game->get_waiting_players().begin(), game->get_waiting_players().end());
    for (const auto& player: all_players)
    {
        QJsonObject player_info;
        player_info["username"] = player->get_name();
        player_info["chips"] = player->get_bank().get_chips();
        player_info["bet"] = player->get_bank().get_bet();
        QJsonArray cards_array = serialize_cards(player, active_player);
        player_info["cards"] = cards_array;
        players_array.append(player_info);
    }
    QJsonObject players_state;
    players_state["players"] = players_array;
    send_all_client(Server::Request_Type::Game_Players_State, players_state);
}

QJsonArray Game_manager::serialize_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player)
{
    QJsonArray card_array;
    for (const Card& card : game->visible_cards(player, active_player))
    {
        QJsonObject card_obj;
        card_obj["card"] = QString::fromStdString(card.get_string_card());
        card_array.append(card_obj);
    }
    return card_array;
}

void Game_manager::log_db_game_session(QString name, std::shared_ptr<Player> &player, int chips)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    int player_id = db->get_player_id(player->get_name());
    int game_id = db->get_game_id(name);
    int bet_amount = player->get_bank().get_chips();
    int win_loss = chips;
    if (!db->insert_into_table(timestamp, player_id, game_id, bet_amount, win_loss)) qDebug() << "Failed to log game session into database.";
}

void Game_manager::send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["response"] = response;
    server->send_packet(socket, packet);
}

void Game_manager::send_all_client(Server::Request_Type request_type, const QJsonObject& response)
{
    for (const auto& player : game->get_players()) if (player->has_socket() && player->get_socket()->isOpen()) send_response(player->get_socket(), request_type, response);
}

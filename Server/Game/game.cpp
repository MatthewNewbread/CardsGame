#include "game.h"
#include "Game/Game-Blackjack/game_blackjack.h"
#include "Game/Game-Poker/game_poker.h"

Game::Game(QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players, bool is_dealer)
    : name(name), min_bet(min_bet), max_players(max_players), max_bots(max_bots), starting_chips(starting_chips), players(players), waiting_players(), is_dealer(is_dealer), status(Status::Ready_To_Start)
{
    pot = 0;
    current_bet = 0;
    count_winners = 0;
    payouts =
    {
        {Result::Win, [&](const int& bank) {return payout_winner(bank);}},
        {Result::Draw, [&](const int& bank) {return payout_tie(bank);}},
        {Result::Lose, [&](const int& bank) {return payout_lose(bank);}}
    };
    turns =
    {
        {Player::Role::Human, [&](std::shared_ptr<Player>& player, const std::vector<std::string>& available_actions) {return emit human_turn(player, available_actions);}},
        {Player::Role::Bot, [&](std::shared_ptr<Player>& player, const std::vector<std::string>& available_actions) {return bot_turn(player, available_actions);}},
        {Player::Role::Dealer, [&](std::shared_ptr<Player>& player, const std::vector<std::string>& available_actions) {return bot_turn(player, available_actions);}}
    };
    place_initial_bet =
    {
        {Player::Role::Human, [&](std::shared_ptr<Player>& player, int min_bet) {emit human_place_bet(player,min_bet);}},
        {Player::Role::Bot, [&](std::shared_ptr<Player>& player, int min_bet) {bot_place_bet(player,min_bet);}},
        {Player::Role::Dealer, [](std::shared_ptr<Player>& , int) {}}
    };
}

void Game::add_user(QTcpSocket *socket, const QString &username)
{
    std::shared_ptr<Player> player = std::make_shared<Player>(username, Player::Role::Human, starting_chips, socket);
    if (is_status(Game::Status::Ready_To_Start))
        players.insert(players.end() - static_cast<int>(is_dealer), player);
    else
        waiting_players.push_back(player);
}

void Game::add_bots(int bots)
{
    for (int i = 0; i < bots; i++) players.insert(players.end() - static_cast<int>(is_dealer), std::make_shared<Player>("Bot_" + std::to_string(i), Player::Role::Bot, starting_chips));
}

void Game::add_dealer()
{
    players.push_back(std::make_shared<Player>("Dealer", Player::Role::Dealer));
}

void Game::disconnect_user(QTcpSocket *socket)
{
    std::remove_if(players.begin(), players.end(), [socket](const std::shared_ptr<Player>& player) {return player->get_socket() == socket;})->get()->set_active(Player::Active::Disconnected);
}

void Game::updating_players()
{
    players.erase(std::remove_if(players.begin(), players.end(), [this](const std::shared_ptr<Player>& player) {return player->get_bank().get_chips() < min_bet || player->is_player_in_active(Player::Active::Disconnected);}), players.end());
    connected_waiting_players();
    restore_bots_if_needed();
}

void Game::connected_waiting_players()
{
    for (const auto& player: waiting_players) if ((int)players.size() < max_players) players.insert(players.end() - static_cast<int>(is_dealer), player);
}

void Game::restore_bots_if_needed()
{
    int current_bots = std::count_if(players.begin(), players.end(), [](const std::shared_ptr<Player>& player) {return player->is_player_in_role(Player::Role::Bot);});
    if (current_bots < max_bots) add_bots(current_bots);
}

bool Game::is_full()
{
    return max_players == (int)players.size();
}

bool Game::is_status(Status status)
{
    return this->status == status;
}

void Game::play()
{
    initialize_round();
    play_round();
    end_round();
}

std::vector<std::shared_ptr<Player> > &Game::get_players() const
{
    return players;
}

std::vector<std::shared_ptr<Player> > &Game::get_waiting_players()
{
    return waiting_players;
}

void Game::initialize_round()
{
    deck.preparation();
    place_initial_bets();
    deal_cards();
}

void Game::end_round()
{
    payment_bet(determine_winners());
    emit game_state();
    pot = 0;
    current_bet = 0;
    updating_players();
    for (auto& player: players) player->clear();
}

void Game::deal_cards()
{
    for(auto& player: players)
    {
        player->get_hand().add_card(deck.deal_card());
        player->get_hand().add_card(deck.deal_card());
    }
}

int Game::determine_winners()
{
    count_winners = 0;
    int score_winner = std::max_element(players.begin(), players.end(), [&](const std::shared_ptr<Player>& first_player, const std::shared_ptr<Player>& second_player)
    {return is_score_winner(first_player, second_player);})->get()->get_hand().get_score();
    for (auto& player: players)
    {
        if (player->get_hand().get_score() == score_winner)
        {
            player->set_result(Player::Results::Winner);
            count_winners++;
        }
        else
        {
            player->set_result(Player::Results::Loser);
        }
    }
    return count_winners;
}

void Game::payment_bet(int count_winners)
{
    for (auto& player: players)
    {
        int bank = payout_strategy(player);
        int chips = player->get_results() == Player::Results::Winner? count_winners > 1? payouts[Result::Draw](bank): payouts[Result::Win](bank): payouts[Result::Lose](bank);
        if (player->has_socket()) emit log_db_game_sessions(name, player, chips);
        player->get_bank().update_chips(chips);
    }
}

void Game::turn(std::shared_ptr<Player> &player)
{
    std::vector<std::string> available_actions = get_available_actions(player);
    turns.find(player->get_role())->second(player, available_actions);
}

void Game::bot_turn(std::shared_ptr<Player>& player, const std::vector<std::string>& available_actions)
{
    QRandomGenerator rng;
    std::string choice = available_actions[rng.bounded(0, available_actions.size() - 1)];
    player_action(player, choice);
}

void Game::player_action(std::shared_ptr<Player>&player, const std::string& action)
{
    actions.find(action)->second(player);
    emit game_state(player);
}

void Game::place_bet(std::shared_ptr<Player> &player, int bet)
{
    pot += bet - player->get_bank().get_bet();
    player->get_bank().take_bet(bet);
}

bool Game::get_is_dealer() const
{
    return is_dealer;
}

int Game::get_min_bet() const
{
    return min_bet;
}

int Game::get_max_players() const
{
    return max_players;
}

int Game::get_max_bots() const
{
    return max_bots;
}

int Game::get_starting_chips() const
{
    return starting_chips;
}

int Game::get_pot() const
{
    return pot;
}

QString Game::get_name() const
{
    return name;
}

void Game::bot_place_bet(std::shared_ptr<Player> &player, int min_bet)
{
    QRandomGenerator rng;
    int bet = rng.bounded(min_bet, player->get_bank().get_chips());
    place_bet(player, bet);
}

std::vector<std::string> Game::get_available_actions(const std::shared_ptr<Player>& player)
{
    std::vector<std::string> available_actions;
    for (const auto& action : actions) if (action_rules.find(action.first)->second(player)) available_actions.push_back(action.first);
    return available_actions;
}

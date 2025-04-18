#include "game_poker.h"
#include "Network/game_manager.h"

Poker::Poker(QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players): Game(name, min_bet, max_players, max_bots, starting_chips, players, false)
{
    dealer_index = 0;
    stages =
    {
        {RoundStage::Flop, [&](){add_cards_to_table(3);}},
        {RoundStage::River, [&](){add_cards_to_table(1);}},
        {RoundStage::Turn, [&](){add_cards_to_table(1);}},
        {RoundStage::Showdown, [&](){showdown();}}
    };
    actions =
    {
        {"Fold", [&](std::shared_ptr<Player>& player) {fold(player);}},
        {"Check", [&](std::shared_ptr<Player>& player) {check(player);}},
        {"Call", [&](std::shared_ptr<Player>& player) {call(player);}},
        {"Raise", [&](std::shared_ptr<Player>& player) {raise(player);}},
        {"All In", [&](std::shared_ptr<Player>& player) {all_in(player);}}
    };
    action_rules =
    {
        {"Fold", [&](const std::shared_ptr<Player>&) {return true;}},
        {"Check", [&](const std::shared_ptr<Player>& player) {return current_bet == player->get_bank().get_bet();}},
        {"Call", [&](const std::shared_ptr<Player>& player) {return player->get_bank().get_bet() < current_bet && player->get_bank().get_chips() >= current_bet;;}},
        {"Raise", [&](const std::shared_ptr<Player>& player) {return player->get_bank().get_chips() >= current_bet;;}},
        {"All In", [&](const std::shared_ptr<Player>&) {return true;}}
    };
}

std::vector<Card> Poker::get_table_cards() const
{
    return table_cards;
}

void Poker::play_round()
{
    table_cards.clear();
    for (const auto& [stage, action]: stages)
    {
        while(!is_round_over())
        {
            for (auto& player: players) if (!player->is_player_in_state(Player::State::Stopped)) turn(player);
            if (is_early_game_over())
            {
                final_after_adding_cards();
                return;
            }
            if (all_players_done()) break;
            restart_betting_round_for_waiting_players();
        }
        action();
        start_new_betting_round();
        if (single_player_left())
        {
            update_score_players();
            break;
        }
    }
    showdown();
}

void Poker::place_initial_bets()
{
    current_bet = min_bet;
    int small_blind_index = dealer_index + 1;
    int big_blind_index = dealer_index + 2;
    int small_blind = current_bet;
    int big_blind = current_bet * 2;
    if (small_blind_index >= (int)players.size()) small_blind_index -= players.size();
    if (big_blind_index >= (int)players.size()) big_blind_index -= players.size();
    place_bet(players[small_blind_index], small_blind);
    place_bet(players[big_blind_index], big_blind);
    pot += small_blind + big_blind;
    dealer_index++;
}

bool Poker::is_score_winner(const std::shared_ptr<Player> &first_player, const std::shared_ptr<Player> &second_player) const
{
    return first_player->get_hand().get_score() < second_player->get_hand().get_score();
}

int Poker::payout_winner(const int &bank)
{
    return bank;
}

int Poker::payout_tie(const int &bank)
{
    return bank / count_winners;
}

int Poker::payout_lose(const int &)
{
    return 0;
}

int Poker::update_cards_value(const std::vector<Card> &cards)
{
    std::vector<std::pair<std::function<bool()>, PokerHand>> vector_hand_rank;
    // Объединяем карты игрока и карты на столе
    std::vector<Card> all_cards = cards;
    all_cards.insert(all_cards.end(), table_cards.begin(), table_cards.end());
    std::sort(all_cards.begin(), all_cards.end());
    // Считаем количество карт каждого ранга
    std::unordered_map<int, int> rank_counts;
    for(const Card& card: all_cards)
    {
        rank_counts[(int)card.get_rank()]++;
    }
    // Определяем старшие ранги
    int highest_rank = (int)all_cards[0].get_rank();
    int second_highest_rank = -1;
    evaluate_hand(highest_rank, second_highest_rank, rank_counts);
    // Проверяем наличие стритов и флешей
    bool straight = is_straight(all_cards);
    bool flush = is_flush(all_cards);
    // Проверяем комбинации в порядке убывания силы
    vector_hand_rank =
    {
        {[&]() { return straight && flush && highest_rank == 14; }, PokerHand::RoyalFlush},
        {[&]() { return straight && flush; }, PokerHand::StraightFlush},
        {[&]() { return rank_counts[highest_rank] == 4; }, PokerHand::FourOfAKind},
        {[&]() { return rank_counts[highest_rank] == 3 && rank_counts[second_highest_rank] == 2; }, PokerHand::FullHouse},
        {[&]() { return flush; }, PokerHand::Flush},
        {[&]() { return straight; }, PokerHand::Straight},
        {[&]() { return rank_counts[highest_rank] == 3; }, PokerHand::ThreeOfAKind},
        {[&]() { return rank_counts[highest_rank] == 2 && rank_counts[second_highest_rank] == 2; }, PokerHand::TwoPair},
        {[&]() { return rank_counts[highest_rank] == 2; }, PokerHand::Pair}
    };
    for (const auto& hand_rank : vector_hand_rank) if (hand_rank.first()) return (int)hand_rank.second + 1;
    return (int)PokerHand::HighCard  + 1;
}

int Poker::payout_strategy(const std::shared_ptr<Player> &) const
{
    return pot;
}

std::vector<Card> Poker::visible_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player)
{
    return (active_player == nullptr || player == active_player)? player->get_hand().get_cards(): std::vector<Card>{{Card::Rank::none, Card::Suit::none}, {Card::Rank::none, Card::Suit::none}};
}

void Poker::fold(std::shared_ptr<Player>& player)
{
    player->get_hand().clear_hand();
    player->set_state(Player::State::Stopped);
}

void Poker::check(std::shared_ptr<Player>& player)
{
    player->set_state(Player::State::Waiting);
}

//СТАВКА
void Poker::call(std::shared_ptr<Player>& player)
{
    place_bet(player, current_bet);
    player->set_state(Player::State::Waiting);
}

//СТАВКА
void Poker::raise(std::shared_ptr<Player>& player)
{
    place_initial_bet.find(player->get_role())->second(player, min_bet);
    current_bet = player->get_bank().get_bet();
    player->set_state(Player::State::Waiting);
}

//СТАВКА
void Poker::all_in(std::shared_ptr<Player>& player)
{
    current_bet = player->get_bank().get_chips() < current_bet? current_bet: player->get_bank().get_chips();
    place_bet(player, current_bet);
    player->set_state(Player::State::Stopped);
}

bool Poker::is_straight(const std::vector<Card> &all_cards)
{
    return std::adjacent_find(all_cards.begin(), all_cards.end(),[](const Card& a, const Card& b) { return (int)b.get_rank() != (int)a.get_rank() + 1;}) == all_cards.end();
}

bool Poker::is_flush(const std::vector<Card> &all_cards)
{
    return std::all_of(all_cards.begin(), all_cards.end(),[&](const Card& card) { return (int)card.get_suit() == (int)all_cards[0].get_suit();});
}

void Poker::evaluate_hand(int &highest_rank, int &second_highest_rank, const std::unordered_map<int, int> &rank_counts)
{
    for(const auto& pair: rank_counts)
    {
        if(pair.first != highest_rank && pair.second > 1)
        {
            second_highest_rank = pair.first;
            break;
        }
    }
}

void Poker::showdown()
{
    update_score_players();

}

bool Poker::single_player_left()
{
    int active_player = 0;
    for (const auto& player: players) if (!player->is_player_in_state(Player::State::Stopped)) active_player++;
    return active_player == 1;
}

bool Poker::all_players_done()
{
    for (const auto& player: players) if (player->get_bank().get_bet() < current_bet && player->is_player_in_state(Player::State::Playing)) return false;
    return true;
}

void Poker::final_after_adding_cards()
{
    add_cards_to_table(5 - (int)table_cards.size());
    showdown();
}

bool Poker::all_in_condition_met()
{
    int active_players = 0;
    for (const auto& player: players) if (is_all_in(player) || player->is_player_in_state(Player::State::Playing)) active_players++;
    return active_players <= 1;
}

bool Poker::is_all_in(const std::shared_ptr<Player>& player) const
{
    return player->get_bank().get_chips() == 0;
}

bool Poker::is_round_over() const
{
    for (const auto& player: players) if (player->is_player_in_state(Player::State::Playing)) return false;
    return true;
}

bool Poker::is_early_game_over() const
{
    int active_players = 0;
    int all_in_players = 0;
    for (const auto& player: players)
    {
        if (!player->is_player_in_state(Player::State::Stopped)) active_players++;
        if (is_all_in(player)) all_in_players++;
    }
    return active_players == 1 && all_in_players > 0;

}

void Poker::start_new_betting_round()
{
    for (auto& player: players) if (!player->is_player_in_state(Player::State::Stopped)) player->set_state(Player::State::Playing);
}

void Poker::restart_betting_round_for_waiting_players()
{
    for (auto& player: players) if (player->is_player_in_state(Player::State::Waiting) && player->get_bank().get_bet() < current_bet) player->set_state(Player::State::Playing);
}

void Poker::update_score_players()
{
    if (!single_player_left())
    {
        for (auto& player: players)
        {
            if (player->is_player_in_state(Player::State::Stopped) && !is_all_in(player)) continue;
            player->get_hand().set_score(update_cards_value(player->get_hand().get_cards()));
        }
    }
    else
    {
        for (auto& player: players)
        {
            if (player->is_player_in_state(Player::State::Stopped)) continue;
            player->get_hand().set_score(1);
        }
    }
}

void Poker::add_cards_to_table(int num_cards)
{
    for (int i = 0; i < num_cards; i++) table_cards.push_back(deck.deal_card());
    emit game_state();
}

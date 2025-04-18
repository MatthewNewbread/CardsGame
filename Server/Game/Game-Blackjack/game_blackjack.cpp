#include "game_blackjack.h"

Blackjack::Blackjack(QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players): Game(name, min_bet, max_players, max_bots, starting_chips, players, true)
{
    card_values =
    {
        {Card::Rank::two, static_cast<int>(Card::Rank::two) + BLACKJACK_VALUE},
        {Card::Rank::three, static_cast<int>(Card::Rank::three) + BLACKJACK_VALUE},
        {Card::Rank::four, static_cast<int>(Card::Rank::four) + BLACKJACK_VALUE},
        {Card::Rank::five, static_cast<int>(Card::Rank::five) + BLACKJACK_VALUE},
        {Card::Rank::six, static_cast<int>(Card::Rank::six) + BLACKJACK_VALUE},
        {Card::Rank::seven, static_cast<int>(Card::Rank::seven) + BLACKJACK_VALUE},
        {Card::Rank::eight, static_cast<int>(Card::Rank::eight) + BLACKJACK_VALUE},
        {Card::Rank::nine, static_cast<int>(Card::Rank::nine) + BLACKJACK_VALUE},
        {Card::Rank::ten, VALUE_HIGHER_CARDS},
        {Card::Rank::jack, VALUE_HIGHER_CARDS},
        {Card::Rank::queen, VALUE_HIGHER_CARDS},
        {Card::Rank::king, VALUE_HIGHER_CARDS},
        {Card::Rank::ace, VALUE_ACE}
    };
    max_range_score =
    {
        {Player::Role::Human, []() {return MAX_RANGE_SCORE_PLAYER;}},
        {Player::Role::Bot, []() {return MAX_RANGE_SCORE_BOT;}},
        {Player::Role::Dealer, []() {return MAX_RANGE_SCORE_DEALER;}}
    };
    actions =
    {
        {"Hit", [&](std::shared_ptr<Player>& player) {hit(player);}},
        {"Double", [&](std::shared_ptr<Player>& player) {double_down(player);}},
        {"Stand", [&](std::shared_ptr<Player>& player) {stand(player);}}
    };
    action_rules =
    {
        {"Hit", [&](const std::shared_ptr<Player>& player) {return !overkill_of_points(player);}},
        {"Double", [&](const std::shared_ptr<Player>& player) {return !overkill_of_points(player) && enough_chips(player) && !player->is_player_in_role(Player::Role::Dealer);}},
        {"Stand", [&](const std::shared_ptr<Player>& player) {return !player->is_player_in_role(Player::Role::Dealer);}}
    };
}

void Blackjack::play_round()
{
    for (auto& player: players)
    {
        while(is_player_finished_turn(player)) turn(player);
        player->set_state(Player::State::Stopped);
    }
}

void Blackjack::place_initial_bets()
{
    for (auto& player: players) place_initial_bet.find(player->get_role())->second(player, min_bet);
}

bool Blackjack::is_score_winner(const std::shared_ptr<Player> &first_player, const std::shared_ptr<Player> &second_player) const
{
    if (first_player->get_hand().get_score() > MAX_RANGE_SCORE_PLAYER) return true;
    if (second_player->get_hand().get_score() > MAX_RANGE_SCORE_PLAYER) return false;
    return first_player->get_hand().get_score() < second_player->get_hand().get_score();
}

int Blackjack::payout_winner(const int& bank)
{
    return bank * WINNER_PAYOUT;
}

int Blackjack::payout_tie(const int& bank)
{
    return bank;
}

int Blackjack::payout_lose(const int& )
{
    return 0;
}

int Blackjack::update_cards_value(const std::vector<Card> &cards)
{
    int score = 0;
    int count_ace = 0;
    for (const Card& card: cards)
    {
        score += card_values.at(card.get_rank());
        if (card.get_rank() == Card::Rank::ace) count_ace++;
    }
    while (score > MAX_RANGE_SCORE_PLAYER && count_ace > 0)
    {
        score -= 10;
        count_ace--;
    }
    return score;
}

int Blackjack::payout_strategy(const std::shared_ptr<Player> &player) const
{
    return player->get_bank().get_bet();
}

std::vector<Card> Blackjack::visible_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player)
{
    return player->get_role() == Player::Role::Dealer && !is_round_over()? std::vector<Card>{player->get_hand().get_cards()[0], {Card::Rank::none, Card::Suit::none}}: player->get_hand().get_cards();
}

bool Blackjack::is_round_over() const
{
    for (const auto& player: players) if (player->get_state() == Player::State::Playing) return false;
    return true;
}

void Blackjack::hit(std::shared_ptr<Player>& player)
{
    player->get_hand().add_card(deck.deal_card());
    player->get_hand().set_score(update_cards_value(player->get_hand().get_cards()));
}

void Blackjack::double_down(std::shared_ptr<Player>& player)
{
    place_bet(player, player->get_bank().get_bet() * DOUBLE_DOWN);
    hit(player);
}

void Blackjack::stand(std::shared_ptr<Player>& player)
{
    player->set_state(Player::State::Waiting);
}

bool Blackjack::is_player_finished_turn(const std::shared_ptr<Player> &player) const
{
    return overkill_of_points(player) || player->is_player_in_state(Player::State::Waiting);
}

bool Blackjack::enough_chips(const std::shared_ptr<Player> &player) const
{
    return player->get_bank().get_bet() * DOUBLE_DOWN <= player->get_bank().get_chips();
}

bool Blackjack::overkill_of_points(const std::shared_ptr<Player> &player) const
{
    return player->get_hand().get_score() >= max_range_score.find(player->get_role())->second();
}

#ifndef GAME_BLACKJACK_H
#define GAME_BLACKJACK_H
#include "Game/game.h"

class Blackjack: public Game
{
public:
    Blackjack(QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players);
protected:
    const static int MAX_RANGE_SCORE_PLAYER = 21;
    const static int MAX_RANGE_SCORE_DEALER = 17;
    const static int MAX_RANGE_SCORE_BOT = 18;
    const static int BLACKJACK_PAYOUT = 3;
    const static int WINNER_PAYOUT = 2;
    const static int DOUBLE_DOWN = 2;
    const static int VALUE_HIGHER_CARDS = 10;
    const static int VALUE_ACE = 11;
    const static int BLACKJACK_VALUE = 2;
    std::map<Card::Rank, int> card_values;
    std::map<Player::Role, std::function<int()>> max_range_score;

    void play_round() override;
    void place_initial_bets() override;
    bool is_score_winner(const std::shared_ptr<Player>& first_player, const std::shared_ptr<Player>& second_player) const override;
    int payout_winner(const int& bank) override;
    int payout_tie(const int& bank) override;
    int payout_lose(const int& bank) override;
    int update_cards_value(const std::vector<Card>& cards) override;
    int payout_strategy(const std::shared_ptr<Player>& player) const override;
    bool is_round_over() const override;
    void hit(std::shared_ptr<Player>& player);
    void double_down(std::shared_ptr<Player>& player);
    void stand(std::shared_ptr<Player>& player);
    bool is_player_finished_turn(const std::shared_ptr<Player> &player) const;
    bool enough_chips(const std::shared_ptr<Player>& player) const;
    bool overkill_of_points(const std::shared_ptr<Player>& player) const;
    std::vector<Card> visible_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player) override;
};

#endif // GAME_BLACKJACK_H

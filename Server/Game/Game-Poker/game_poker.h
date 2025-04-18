#ifndef GAME_POKER_H
#define GAME_POKER_H
#include <unordered_map>
#include "Game/game.h"

class Poker: public Game
{
public:
    enum class RoundStage {Flop, River, Turn, Showdown};
    enum class PokerHand {HighCard, Pair, TwoPair, ThreeOfAKind, Flush, Straight, FullHouse, FourOfAKind, StraightFlush, RoyalFlush};

    Poker(QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players);
    std::vector<Card> get_table_cards() const;
    std::vector<Card> visible_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player) override;
protected:
    static const int MAX_ROUND = 4;
    static int dealer_index;

    std::vector<Card> table_cards;
    std::map<RoundStage, std::function<void()>> stages;

    void play_round() override;
    void place_initial_bets() override;
    bool is_score_winner(const std::shared_ptr<Player>& first_player, const std::shared_ptr<Player>& second_player) const override;
    int payout_winner(const int& bank) override;
    int payout_tie(const int& bank) override;
    int payout_lose(const int& bank) override;
    int update_cards_value(const std::vector<Card>& cards) override;
    int payout_strategy(const std::shared_ptr<Player>& player) const override;
    bool is_round_over() const override;
    void fold(std::shared_ptr<Player>& player);
    void check(std::shared_ptr<Player>& player);
    void call(std::shared_ptr<Player>& player);
    void raise(std::shared_ptr<Player>& player);
    void all_in(std::shared_ptr<Player>& player);
    void add_cards_to_table(int count);
    bool is_straight(const std::vector<Card> &all_cards);
    bool is_flush(const std::vector<Card> &all_cards);
    void evaluate_hand(int &highest_rank, int &second_highest_rank, const std::unordered_map<int, int> &rank_counts);
    void showdown();
    bool single_player_left();
    bool all_players_done();
    void final_after_adding_cards();
    bool all_in_condition_met();
    bool is_all_in(const std::shared_ptr<Player>& player) const;
    bool is_early_game_over() const;
    void start_new_betting_round();
    void restart_betting_round_for_waiting_players();
    void update_score_players();
};

#endif // GAME_POKER_H

#ifndef GAME_H
#define GAME_H
#include <QRandomGenerator>
#include "Game/Deck/deck.h"
#include "Game/Player/player.h"

class Game_manager;

class Game
{
public:
    enum class Result {Win, Draw, Lose};
    enum class Status {Ready_To_Start, Player_Turn, Game_Over};

    Game(QString name, int min_bet, int max_players, int max_bots, int starting_chips, std::vector<std::shared_ptr<Player>>& players, bool is_dealer);

    void add_user(QTcpSocket *socket, const QString &username);
    void add_bots(int bots);
    void add_dealer();
    void disconnect_user(QTcpSocket* socket);
    void updating_players();
    void connected_waiting_players();
    void restore_bots_if_needed();
    bool is_full();
    bool is_status(Status status);

    QString get_name() const;
    int get_min_bet() const;
    int get_max_players() const;
    int get_max_bots() const;
    int get_starting_chips() const;
    int get_pot() const;
    bool get_is_dealer() const;
    std::vector<std::shared_ptr<Player>>& get_players() const;
    std::vector<std::shared_ptr<Player>>& get_waiting_players();

    void play();
    void player_action(std::shared_ptr<Player>&player, const std::string& action);
    void place_bet(std::shared_ptr<Player>& player, int bet);
    std::vector<std::string> get_available_actions(const std::shared_ptr<Player>& player);
    virtual std::vector<Card> visible_cards(const std::shared_ptr<Player> &player, const std::shared_ptr<Player> &active_player) = 0;
signals:
    void log_db_game_sessions(QString name, std::shared_ptr<Player>& player, int chips);
    void human_turn(std::shared_ptr<Player>& player, const std::vector<std::string>& available_actions);
    void human_place_bet(std::shared_ptr<Player>& player, int min_bet);
    void game_state(const std::shared_ptr<Player>& active_player = nullptr);
protected:
    QString name;
    int min_bet, max_players, max_bots, starting_chips, pot, current_bet, count_winners;
    Deck deck;
    std::vector<std::shared_ptr<Player>>& players;
    std::vector<std::shared_ptr<Player>> waiting_players;
    std::map<std::string, std::function<void(std::shared_ptr<Player>&)>> actions;
    std::map<std::string, std::function<bool(const std::shared_ptr<Player>&)>> action_rules;
    std::map<Result, std::function<int(const int&)>> payouts;
    std::map<Player::Role, std::function<void(std::shared_ptr<Player>&, const std::vector<std::string>&)>> turns;
    std::map<Player::Role, std::function<void(std::shared_ptr<Player>& player, int min_bet)>> place_initial_bet;
    bool is_dealer;
    Status status;

    void initialize_round();
    void end_round();
    void deal_cards();
    int determine_winners();
    void payment_bet(int count_winners);
    void turn(std::shared_ptr<Player>& player);
    void bot_turn(std::shared_ptr<Player>& player, const std::vector<std::string>& available_actions);
    void bot_place_bet(std::shared_ptr<Player>& player, int min_bet);
    virtual void play_round() = 0;
    virtual void place_initial_bets() = 0;
    virtual bool is_score_winner(const std::shared_ptr<Player>& first_player, const std::shared_ptr<Player>& second_player) const = 0;
    virtual int payout_winner(const int& bank) = 0;
    virtual int payout_tie(const int& bank) = 0;
    virtual int payout_lose(const int& bank) = 0;
    virtual int update_cards_value(const std::vector<Card>& cards) = 0;
    virtual int payout_strategy(const std::shared_ptr<Player>& player) const = 0;
    virtual bool is_round_over() const = 0;
};

#endif // GAME_H

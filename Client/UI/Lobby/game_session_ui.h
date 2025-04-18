#ifndef GAME_SESSION_UI_H
#define GAME_SESSION_UI_H
#include <QObject>

class Game_session_ui: public QObject
{
    Q_OBJECT
public:
    QString game_name;
    int max_players;
    int current_players;
    int count_bots;
    int min_bet;
    int starting_chips;

    Game_session_ui();
    Game_session_ui(QString game_name, int max_players, int current_players, int count_bots, int min_bet, int starting_chips);
};

#endif // GAME_SESSION_UI_H

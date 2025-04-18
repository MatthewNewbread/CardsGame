#ifndef GAME_TABLE_UI_H
#define GAME_TABLE_UI_H
#include <QObject>

class Game_table_ui: public QObject
{
    Q_OBJECT
public:
    QString table_name;
    int seats;
    int taken_seats;
    int min_bet;
    int starting_chips;
    QString game_type;

    Game_table_ui();
    Game_table_ui(QString table_name, int seats, int taken_seats, int min_bet, int starting_chips, QString game_type);
};

#endif // GAME_TABLE_UI_H

#ifndef TABLE_UI_H
#define TABLE_UI_H
#include <iostream>
#include "error_handler.h"
#include "Network/table_manager.h"

class Table_ui: public QObject
{
    Q_OBJECT
public:
    Table_ui(Table_manager* table_manager, QObject* parent = nullptr);
    void configure_table_settings();
    void choose_game();
    void choose_type_table();
    void choose_count_bots();
    void choose_starting_chips();
    void display_games() const;
    void display_type_table() const;
private slots:
    void update_table_info(int min_bet, int max_players, QStringList games_list, QStringList table_type_list);
private:
    Table_manager* table_manager;
    int min_bet;
    int max_players;
    QStringList games_list;
    QStringList table_type_list;
};

#endif // TABLE_UI_H

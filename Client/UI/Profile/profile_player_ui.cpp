#include "profile_player_ui.h"

Profile_Player_ui::Profile_Player_ui(Client *client, QWidget *parent): Profile_main_ui(client, parent)
{
    availableTables = {Database_manager::Table::Users, Database_manager::Table::Game_Sessions};
}

void Profile_Player_ui::updateData()
{
    if (currentTable == Database_manager::Table::Users)
        client->get_db_manager()->view_own_records(client->get_username());
    else
        client->get_db_manager()->view_table(currentTable);
}

void Profile_Player_ui::contextMenuEvent(QContextMenuEvent *)
{

}

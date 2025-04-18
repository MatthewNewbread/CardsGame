#include "profile_manager_ui.h"

Profile_Manager_ui::Profile_Manager_ui(Client *client, QWidget *parent): Profile_main_ui(client, parent)
{
    availableTables = {Database_manager::Table::Users, Database_manager::Table::Games, Database_manager::Table::Game_Sessions, Database_manager::Table::Roles};
}

void Profile_Manager_ui::updateData()
{
    client->get_db_manager()->view_table(currentTable);
}

void Profile_Manager_ui::contextMenuEvent(QContextMenuEvent *)
{

}

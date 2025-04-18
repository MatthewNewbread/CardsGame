#include "profile_admin_ui.h"

Profile_Admin_ui::Profile_Admin_ui(Client *client, QWidget *parent): Profile_main_ui(client, parent)
{
    availableTables = {Database_manager::Table::Users, Database_manager::Table::Games, Database_manager::Table::Game_Sessions, Database_manager::Table::Roles};
}

void Profile_Admin_ui::updateData()
{
    client->get_db_manager()->view_table(currentTable);
}

void Profile_Admin_ui::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = tableView->indexAt(event->pos());
    if (!index.isValid()) return;
    QMenu contextMenu(this);
    QAction *deleteUserAction = new QAction("Delete User", this);
    QAction *changeRoleAction = new QAction("Change Role", this);
    connect(deleteUserAction, &QAction::triggered, this, [this, index]() {int user_id = tableModel->item(index.row(), 0)->text().toInt(); deleteUser(user_id);});
    connect(changeRoleAction, &QAction::triggered, this, [this, index]() { int user_id = tableModel->item(index.row(), 0)->text().toInt(); changeUserRole(user_id);});
    contextMenu.addAction(deleteUserAction);
    contextMenu.addAction(changeRoleAction);
    contextMenu.exec(event->globalPos());
}

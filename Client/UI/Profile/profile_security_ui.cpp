#include "profile_security_ui.h"

Profile_Security_ui::Profile_Security_ui(Client *client, QWidget *parent): Profile_main_ui(client, parent)
{
    availableTables = {Database_manager::Table::Users};
    nextTableButton->hide();
    prevTableButton->hide();
}

void Profile_Security_ui::updateData()
{
    client->get_db_manager()->view_table(Database_manager::Table::Users);
}

void Profile_Security_ui::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = tableView->indexAt(event->pos());
    if (!index.isValid()) return;
    QMenu contextMenu(this);
    QAction *deleteUserAction = new QAction("Delete User", this);
    connect(deleteUserAction, &QAction::triggered, this, [this, index]() { int user_id = tableModel->item(index.row(), 0)->text().toInt(); deleteUser(user_id);});
    contextMenu.addAction(deleteUserAction);
    contextMenu.exec(event->globalPos());
}


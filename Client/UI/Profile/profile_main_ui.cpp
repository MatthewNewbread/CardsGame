#include "UI/Profile/profile_main_ui.h"

Profile_main_ui::Profile_main_ui(Client *client, QWidget *parent): QWidget(parent), client(client)
{
    roles =
    {
        {Database_manager::Role::Player, "Player"},
        {Database_manager::Role::Manager, "Manager"},
        {Database_manager::Role::Security, "Security"},
        {Database_manager::Role::Admin, "Admin"}
    };
    enum_roles =
    {
        {"Player", Database_manager::Role::Player},
        {"Manager", Database_manager::Role::Manager},
        {"Security", Database_manager::Role::Security},
        {"Admin", Database_manager::Role::Admin}
    };
    table_header =
    {
        {Database_manager::Table::Users, {"ID", "Username", "Password", "Email", "Data_on_birth", "Role_ID", "Online"}},
        {Database_manager::Table::Game_Sessions, {"ID", "Timestamp", "User_ID", "Game_ID", "Bet_amount", "Win_loss"}},
        {Database_manager::Table::Games, {"ID", "Name", "Description"}},
        {Database_manager::Table::Roles, {"ID", "Name"}}
    };
    initialize();
}

void Profile_main_ui::setupUI()
{
    tableView = new QTableView(this);
    proxyModel = new QSortFilterProxyModel(this);
    tableModel = new QStandardItemModel(this);
    proxyModel->setSourceModel(tableModel);
    tableView->setModel(proxyModel);

    nextTableButton = new QPushButton("Next Table", this);
    prevTableButton = new QPushButton("Previous Table", this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableView);
    mainLayout->addWidget(prevTableButton);
    mainLayout->addWidget(nextTableButton);

    setLayout(mainLayout);

    // Настраиваем таймер для периодического обновления данных
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &Profile_main_ui::updateData);
    updateTimer->start(5000);  // Обновление каждые 5 секунд
}

void Profile_main_ui::setupConnections()
{
    connect(nextTableButton, &QPushButton::clicked, this, &Profile_main_ui::onNextTable);
    connect(prevTableButton, &QPushButton::clicked, this, &Profile_main_ui::onPrevTable);
    connect(tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &Profile_main_ui::onHeaderClicked);
    connect(client->get_db_manager(), &Database_manager::records_received, this, &Profile_main_ui::loadTableData);
}

void Profile_main_ui::loadTableData(const QVector<QVariantMap>& records)
{
    tableModel->clear();
    tableModel->setHorizontalHeaderLabels(table_header[currentTable]);
    for (const QVariantMap &record : records)
    {
        QList<QStandardItem *> rowItems;
        for (const QString &key : table_header[currentTable]) rowItems.append(new QStandardItem(record[key].toString()));
        tableModel->appendRow(rowItems);
    }
}

void Profile_main_ui::updateData()
{
    client->get_db_manager()->view_table(currentTable);
}

void Profile_main_ui::contextMenuEvent(QContextMenuEvent *) {}

void Profile_main_ui::initialize()
{
    setupUI();
    updateData();
    setupConnections();
}

void Profile_main_ui::clearSort()
{
    sortedColumn = -1;
    sortOrder = Qt::AscendingOrder;
}

void Profile_main_ui::onHeaderClicked(int column)
{
    if (sortedColumn == column)
    {
        sortOrder = (sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    }
    else
    {
        sortOrder = Qt::AscendingOrder;
        sortedColumn = column;
    }
    proxyModel->sort(column, sortOrder);
}

void Profile_main_ui::deleteUser(int user_id)
{
    client->get_db_manager()->delete_user(user_id);
}

void Profile_main_ui::changeUserRole(int user_id)
{
    bool ok;
    QList<QString> availableRoles;
    for (const QString& role: availableRoles) availableRoles.append(role);
    QString selectedRole = QInputDialog::getItem(this, "Change Role", "Select new role:", availableRoles, 0, false, &ok);
    if (!ok || selectedRole.isEmpty()) return;
    Database_manager::Role newRole = enum_roles[selectedRole];
    client->get_db_manager()->update_user_role(user_id, static_cast<int>(newRole));
}

void Profile_main_ui::onNextTable()
{
    currentTableIndex = (currentTableIndex + 1) % availableTables.size();
    currentTable = availableTables[currentTableIndex];
    updateData();
    clearSort();
}

void Profile_main_ui::onPrevTable()
{
    currentTableIndex = (currentTableIndex - 1 + availableTables.size()) % availableTables.size();
    currentTable = availableTables[currentTableIndex];
    updateData();
    clearSort();
}

void Profile_main_ui::handleUserDeleted(bool success)
{
    if (success)
        updateData();
    else
        QMessageBox::warning(this, "Error", "Failed to delete user.");
}

void Profile_main_ui::handleUserRoleUpdated(bool success)
{
    if (success)
        updateData();
    else
        QMessageBox::warning(this, "Error", "Failed to change role.");
}

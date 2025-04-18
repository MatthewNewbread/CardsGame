#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QMenu>
#include <QVBoxLayout>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QHeaderView>
#include <QAction>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QContextMenuEvent>
#include <QInputDialog>
#include "Network/database_manager.h"

class Profile_main_ui : public QWidget
{
    Q_OBJECT
public:
    explicit Profile_main_ui(Client* client, QWidget *parent = nullptr);
    virtual ~Profile_main_ui() = default;
protected:
    Client* client;

    Database_manager::Table currentTable;
    int currentTableIndex;
    int sortedColumn;
    Qt::SortOrder sortOrder;

    QTableView *tableView;
    QStandardItemModel *tableModel;
    QSortFilterProxyModel *proxyModel;
    QPushButton *nextTableButton;
    QPushButton *prevTableButton;
    QTimer *updateTimer;
    QContextMenuEvent* event;

    QMap<Database_manager::Role, QString> roles;
    QMap<QString, Database_manager::Role> enum_roles;
    QVector<Database_manager::Table> availableTables;
    QMap<Database_manager::Table, QList<QString>> table_header;

    virtual void updateData();
    virtual void contextMenuEvent(QContextMenuEvent* event);
    void initialize();
    void setupUI();
    void setupConnections();
    void clearSort();
    void onHeaderClicked(int column);
    void deleteUser(int user_id);
    void changeUserRole(int user_id);

private slots:
    void loadTableData(const QVector<QVariantMap>& records);
    void onNextTable();
    void onPrevTable();
    void handleUserDeleted(bool success);
    void handleUserRoleUpdated(bool success);
};

//#ifndef PROFILE_UI_H
//#define PROFILE_UI_H
//#include <QWidget>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <QLabel>
//#include <QMenu>
//#include <QMessageBox>
//#include <QPushButton>
//#include <QTableView>
//#include <QComboBox>
//#include <QLineEdit>
//#include <QHeaderView>
//#include <QSortFilterProxyModel>
//#include <QStandardItemModel>
//#include "Network/database_manager.h"

//class Profile_ui: public QWidget
//{
//    Q_OBJECT
//public:
//    Profile_ui(Database_manager* db_manager, Client* client, QWidget* parent = nullptr);
//private slots:
//    void on_records_received(const QVector<QVariantMap>& records);
//    void on_user_deleted(bool success);
//    void on_user_role_updated(bool success);
//    void handle_view_own_records();
//    void handle_view_table();
//    void handle_header_clicked(int logical_index);
//    void show_context_menu(const QPoint &pos);

//private:
//    Database_manager* db_manager;
//    Client* client;
//    QMap<Client::User_Role, QString> string_role;
//    QMap<Database_manager::Filters, std::function<void(Qt::SortOrder order)>> filters;

//    QLabel* username_label;
//    QPushButton* main_menu_button;
//    QTableView* table_view;
//    QStandardItemModel* table_model;
//    QSortFilterProxyModel* proxy_model;
//    QComboBox* table_combo_box;
//    QComboBox* filter_combo_box;
//    QComboBox* role_combo_box;
//    QLineEdit* filter_line_edit;
//    QPushButton* filter_button;
//    QPushButton* delete_user_button;
//    QPushButton* update_user_role_button;

//    void setup_UI();
//    void update_table(const QVector<QVariantMap>& records);
//    void confirm_delete_user(const QModelIndex& index);
//    void confirm_update_user_role(const QModelIndex& index, Client::User_Role new_role);
//    QString role_to_string(Client::User_Role role) const;
//    int get_user_id_from_index(const QModelIndex& index) const;
//    void handle_role_based_access();
//    void initialize_string_role();
//    void initialize_filters();
//};

//#endif // PROFILE_UI_H

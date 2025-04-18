#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H
#include "client.h"

class Database_manager: public QObject
{
    Q_OBJECT
public:
    enum class Role { Player, Manager, Security, Admin};
    enum class Table { Users, Games, Game_Sessions, Roles};

    explicit Database_manager(Client* client, QObject* parent = nullptr);
    void view_own_records(const QString& username);
    void view_table(Table table);
    void delete_user(int user_id);
    void update_user_role(int user_id, int role_id);
    void user_role(const QString& username);
signals:
    void records_received(const QVector<QVariantMap>& records);
    void user_deleted(bool success);
    void user_role_updated(bool success);
    void user_role_received(Role role);
private:
    Client* client;
    void send_request(Client::Request_Type request_type, const QJsonObject& json_data);
};

#endif // DATABASE_MANAGER_H

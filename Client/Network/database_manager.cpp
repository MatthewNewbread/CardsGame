#include "database_manager.h"

Database_manager::Database_manager(Client *client, QObject *parent): QObject(parent), client(client)
{
}

void Database_manager::view_own_records(const QString &username)
{
    QJsonObject json_data;
    json_data["username"] = username;
    send_request(Client::Request_Type::Model_player_data, json_data);
}

void Database_manager::view_table(Table table)
{
    QJsonObject json_data;
    json_data["table"] = static_cast<int>(table);
    send_request(Client::Request_Type::Model_all_records, json_data);
}

void Database_manager::delete_user(int user_id)
{
    QJsonObject json_data;
    json_data["user_id"] = user_id;
    send_request(Client::Request_Type::Model_delete_user, json_data);
}

void Database_manager::update_user_role(int user_id, int role_id)
{
    QJsonObject json_data;
    json_data["user_id"] = user_id;
    json_data["role_id"] = role_id;
    send_request(Client::Request_Type::Model_update_user_role, json_data);
}

void Database_manager::user_role(const QString& username)
{
    QJsonObject json_data;
    json_data["username"] = username;
    send_request(Client::Request_Type::Model_Role_User, json_data);
}

void Database_manager::send_request(Client::Request_Type request_type, const QJsonObject& json_data)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["data"] = json_data;
    QJsonDocument doc(packet);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    client->send_request(data);
}

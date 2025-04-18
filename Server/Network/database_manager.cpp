#include "database_manager.h"

Database_manager::Database_manager(Server* server, Database_model *model, QObject *parent): QObject(parent), server(server), model(model)
{}

void Database_manager:: process_view_own_records(QTcpSocket* socket, const QString& username)
{
    QMutexLocker locker(&db_mutex);
    QVector<QVariantMap> records = model->get_player_data(username);
    QJsonObject response;
    QJsonArray records_array;
    for (const QVariantMap& record : records)
    {
        QJsonObject record_obj = QJsonObject::fromVariantMap(record);
        records_array.append(record_obj);
    }
    response["records"] = records_array;
    send_response(socket, Server::Request_Type::Model_player_data, response);
}

void Database_manager::process_view_table(QTcpSocket* socket, const DataBase::Table& name_table)
{
    QMutexLocker locker(&db_mutex);
    QVector<QVariantMap> records = model->get_all_records(name_table);
    QJsonObject response;
    QJsonArray records_array;
    for (const QVariantMap& record : records)
    {
        QJsonObject record_obj = QJsonObject::fromVariantMap(record);
        records_array.append(record_obj);
    }
    response["records"] = records_array;
    send_response(socket, Server::Request_Type::Model_all_records, response);
}

void Database_manager::process_delete_user(QTcpSocket* socket, int user_id)
{
    QMutexLocker locker(&db_mutex);
    bool success = model->delete_user(user_id);
    QJsonObject response;
    response["success"] = success;
    send_response(socket, Server::Request_Type::Model_delete_user, response);
}

void Database_manager::process_update_user_role(QTcpSocket* socket, int user_id, int role_id)
{
    QMutexLocker locker(&db_mutex);
    bool success = model->update_user_role(user_id, role_id);
    QJsonObject response;
    response["success"] = success;
    send_response(socket, Server::Request_Type::Model_update_user_role, response);
}

void Database_manager::process_user_role(QTcpSocket *socket, const QString &username)
{
    QMutexLocker locker(&db_mutex);
    QJsonObject response;
    int role = model->get_role_by_username(username);
    response["role"] = role;
    send_response(socket, Server::Request_Type::Model_Role_User, response);
}

void Database_manager::send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["response"] = response;
    server->send_packet(socket, packet);
}

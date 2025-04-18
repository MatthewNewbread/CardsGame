#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H
#include "server.h"
#include "Database/database_model.h"

class Database_manager: public QObject
{
    Q_OBJECT
public:
    explicit Database_manager(Server* server, Database_model* model, QObject* parent = nullptr);
    void process_view_own_records(QTcpSocket* socket, const QString& username);
    void process_view_table(QTcpSocket* socket, const DataBase::Table& name_table);
    void process_delete_user(QTcpSocket* socket, int user_id);
    void process_update_user_role(QTcpSocket* socket, int user_id, int role_id);
    void process_user_role(QTcpSocket* socket, const QString& username);
private:
    Server* server;
    Database_model* model;
    QMutex db_mutex;

    void send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response);
};

#endif // DATABASE_MANAGER_H

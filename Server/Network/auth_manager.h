#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H
#include "server.h"
#include "auth.h"

class Auth_manager: public QObject
{
    Q_OBJECT
public:
    explicit Auth_manager(Server* server, QObject* parent = nullptr);
    void process_auth_request(QTcpSocket* socket, const QString& username, const QString& password);
    void process_register_request(QTcpSocket* socket, const QString& username, const QString& password, const QString& email, const QDate& birth_date);
private:
    Server* server;
    Auth* auth_logic;
    QMutex auth_mutex;

    void send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response);
};

#endif // AUTH_MANAGER_H

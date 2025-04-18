#include "auth_manager.h"
#include "server.h"

Auth_manager::Auth_manager(Server* server, QObject* parent): QObject(parent), server(server), auth_logic()
{}

void Auth_manager::process_auth_request(QTcpSocket *socket, const QString &username, const QString &password)
{
    QMutexLocker locker(&auth_mutex);
    bool success = auth_logic->authenticate(username, password);
    QJsonObject response;
    response["username"] = username;
    response["success"] = success;
    send_response(socket, Server::Request_Type::Authenticate, response);
}

void Auth_manager::process_register_request(QTcpSocket *socket, const QString &username, const QString &password, const QString &email, const QDate &birth_date)
{
    QMutexLocker locker(&auth_mutex);
    bool success = auth_logic->register_user(username, password, email, birth_date, DataBase::Role::Player);
    QJsonObject response;
    response["username"] = username;
    response["success"] = success;
    send_response(socket, Server::Request_Type::Register, response);
}

void Auth_manager::send_response(QTcpSocket* socket, Server::Request_Type request_type, const QJsonObject& response)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["response"] = response;
    server->send_packet(socket, packet);
}

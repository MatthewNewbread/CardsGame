#include "auth_manager.h"

Auth_manager::Auth_manager(QObject *parent, Client *client): QObject(parent), client(client)
{}

void Auth_manager::send_request(Client::Request_Type request_type, const QJsonObject& json_data)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["data"] = json_data;
    QJsonDocument doc(packet);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    client->send_request(data);
}

void Auth_manager::login_user(const QString &username, const QString &password)
{
    QJsonObject json_data;
    json_data["username"] = username;
    json_data["password"] = password;
    send_request(Client::Request_Type::Authenticate, json_data);
}

void Auth_manager::registration_user(const QString& username, const QString& password, const QString& email, QDate& birth_date)
{
    QJsonObject json_data;
    json_data["username"] = username;
    json_data["password"] = password;
    json_data["email"] = email;
    json_data["birth_date"] = birth_date.toString(Qt::ISODate);
    send_request(Client::Request_Type::Register, json_data);
}

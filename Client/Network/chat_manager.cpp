#include "chat_manager.h"

Chat_manager::Chat_manager(Client* client, QObject *parent): QObject(parent), client(client)
{}

void Chat_manager::send_message(const QString &message)
{
    QJsonObject data;
    data["username"] = client->get_username();
    data["message"] = message;
    send_request(Client::Request_Type::Send_Message, data);
}

void Chat_manager::send_request(Client::Request_Type request_type, const QJsonObject& json_data)
{
    QJsonObject packet;
    packet["request_type"] = static_cast<int>(request_type);
    packet["data"] = json_data;
    QJsonDocument doc(packet);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    client->send_request(data);
}

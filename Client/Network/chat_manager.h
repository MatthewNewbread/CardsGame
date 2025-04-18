#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H
#include "client.h"

class Chat_manager: public QObject
{
    Q_OBJECT
public:
    explicit Chat_manager(Client* client, QObject* parent = nullptr);
    void send_message(const QString& message);
signals:
    void messag_received(const QString &time, const QString &username, const QString &message);
private:
    Client* client;
    void send_request(Client::Request_Type request_type, const QJsonObject& json_data);
};

#endif // CHAT_MANAGER_H

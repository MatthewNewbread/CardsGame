#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H
#include "client.h"

class Auth_manager: public QObject
{
    Q_OBJECT
public:
    explicit Auth_manager(QObject* parent = nullptr, Client* client = nullptr);
    void login_user(const QString& username, const QString& password);
    void registration_user(const QString& username, const QString& password, const QString& email, QDate& birth_date);
signals:
    void auth_success(QString username);
    void auth_failure();
private:
    Client* client;
    void send_request(Client::Request_Type request_type, const QJsonObject& json_data);
};

#endif // AUTH_MANAGER_H

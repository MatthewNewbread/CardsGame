#ifndef USER_H
#define USER_H
#include <QString>
#include <QDate>
#include <QTcpSocket>
#include <memory>

class User
{
public:


    User(const QString& username, const QString& password, const QString& email, const QDate& birth_date, Role role, QTcpSocket* socket);
    QString get_username() const;
    QString get_password() const;
    QString get_email() const;
    QDate get_birth_date() const;
    Role get_role() const;
    QTcpSocket* get_socket() const;
private:
    QString nickname;           // Имя пользователя для других пользователей
    QString username;           // Уникальный идентификатор пользователя
    QString password;           // hashed
    QString email;              // hashed
    QDate birth_date;
    Role role;
    QTcpSocket* socket;
};

#endif

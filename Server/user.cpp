#include "user.h"

User::User(const QString& username, const QString& password, const QString& email, const QDate& birth_date, Role role, QTcpSocket* socket):
    username{username}, password{password}, email{email}, birth_date{birth_date}, role{role}, socket{socket}
{
}

QString User::get_username() const {return username;}
QString User::get_password() const {return password;}
QString User::get_email() const {return email;}
QDate User::get_birth_date() const {return birth_date;}
User::Role User::get_role() const {return role;}
QTcpSocket* User::get_socket() const {return socket;}

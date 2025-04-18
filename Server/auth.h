#ifndef AUTH_H
#define AUTH_H
#include "Database/database.h"
#include "file_logger.h"

class Auth
{
public:
    Auth();
    bool authenticate(const QString& username, const QString& password);
    bool register_user(const QString& username, const QString& password, const QString& email, const QDate& birth_date, DataBase::Role role);
    void logout(const QString& email);
private:
    DataBase db;
};

#endif // AUTH_H

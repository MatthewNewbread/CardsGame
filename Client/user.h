#ifndef USER_H
#define USER_H
#include <QString>
#include <QDate>

class User
{
public:
    enum class Role { Player, Manager, Security, Admin};

    User(const QString& username, const Role& role);
    QString get_username() const;
    Role get_role() const;
private:
    QString username;
    Role role;
};

#endif // USER_H

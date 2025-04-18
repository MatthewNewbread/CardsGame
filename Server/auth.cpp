#include "auth.h"

Auth::Auth(): db{}
{

}

bool Auth::authenticate(const QString &username, const QString &password)
{
    if (db.is_user_online(username))
    {
        File_Logger::get_instance(File_Logger::Name_File::User_Activity).log(username, "Failed login attempt: user already online");
        return false; // Пользователь уже онлайн
    }
    QByteArray encrypted_password = db.encrypt(password);
    if (db.authenticate_user(username, encrypted_password) && db.set_user_online_status(username, true))
    {
        File_Logger::get_instance(File_Logger::Name_File::User_Activity).log(username, "User logged in.");
        return true;
    }
    return false;
}

bool Auth::register_user(const QString &username, const QString &password, const QString &email, const QDate &birth_date, DataBase::Role role)
{
    if (!db.create_user(username, password, email, birth_date, role)) return false;
    File_Logger::get_instance(File_Logger::Name_File::User_Activity).log(username, "User registered.");
    return true;
}

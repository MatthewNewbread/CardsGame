#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDate>
#include <iostream>
#include <map>
#include <functional>
#include "Qt-AES-master/qaesencryption.h"
#include "user.h"
#include "error_handler.h"

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "Casino_DataBase"
#define DATABASE_NAME       "Casino.db"

#define USERS                   "Users"             // Название таблицы
#define USERS_NAME              "Username"          // Вторая колонка - Юзернейм
#define USERS_PASSWORD          "Password"          // Третья колонка - Пароль
#define USERS_EMAIL             "Email"             // Четвертая колонка - Почта
#define USERS_DATA_ON_BIRTH     "Data_on_birth"     // Пятая колонка - Дата рождения
#define USERS_ROLE              "Role_ID"           // Шестая колонка - Роль
#define USERS_ONLINE            "Online"            // Седьмая колонка - Онлайн

#define GAMES                   "Games"             // Название таблицы
#define GAMES_NAME              "Name"              // Вторая колонка - Название
#define GAMES_DESCRIPTION       "Description"       // Третья колонка - Описание

#define GAME_SESSIONS            "Game_Sessions"    // Название таблицы Game_Sessions
#define GAME_SESSIONS_TIMESTAMP  "Timestamp"        // Вторая колонка - Время игры
#define GAME_SESSIONS_USER_ID    "User_ID"          // Третья колонка - ID юзера (внешний ключ)
#define GAME_SESSIONS_GAME_ID    "Game_ID"          // Четвертая колонка - ID игры (внешний ключ)
#define GAME_SESSIONS_BET_AMOUNT "Bet_amount"       // Пятая колонка - Внесено в игру фишек
#define GAME_SESSIONS_WIN_LOSS   "Win_loss"         // Шестая колонка - Заработок игрока с игры

#define ROLES                    "Roles"            // Название таблицы
#define ROLES_NAME               "Name"             // Вторая колонка - Название

class DataBase: public QObject
{
    Q_OBJECT
public:
    enum class Role { Player = 1, Manager, Security, Admin};
    enum class Table { Users, Games, Game_Sessions, Roles};
    enum class Query_Error_Type { Create_Table, Insert_Into_Table, Remove_Record, Get_Data};

    QMap<Table, QString> tables;

    explicit DataBase(QObject* parent = nullptr);
    void connect_to_database();
    QByteArray encrypt(const QString& data);
    QString decrypt(const QByteArray& data);
    bool is_user_exists(const QString& username);
    bool create_user(const QString &username, const QString &password, const QString &email, const QDate &birth_date, Role role);
    bool set_user_online_status(const QString& username, const bool& online);
    bool is_user_online(const QString& username);
    bool authenticate_user(const QString& username, const QByteArray& encrypted_password);
    bool insert_into_table(const QString& username, const QString& password, const QString& email, const QDate& date, const Role& role);
    bool insert_into_table(const QString& game_name, const QString& description);
    bool insert_into_table(const QString& role_name);
    bool insert_into_table(const QString& timestamp, const int& user_id, const int& game_id, const int& bet_amount, const int& win_loss);
    bool query_remove_record(Table table, const int id);
    bool execute_query(QSqlQuery& query, const QString& username, const QString& error_message);
    bool check_game_in_db(const QString& name);
    int get_player_id(const QString& username);
    int get_game_id(const QString& name);
private:
    QSqlDatabase db;
    Error_Handler* error_handler;
    const QByteArray encryption_key = "harder0better1faster4stronger";
    std::map<Table, QString> queries_create_table;
    std::map<Table, QString> queries_insert_into_table;
    std::map<Table, QString> queries_remove_record;
    std::map<Query_Error_Type, QString> query_error;
    std::map<QVariant, std::function<void(QSqlQuery& query, const QString& key, QVariant& value)>> value_for_request;

    bool create_table(Table table, QSqlQuery& query);
    bool insert_into_table(Table table, QSqlQuery& query);
    bool query_for_insert_into_table(Table table, const QVariantMap& data);
    bool query_for_create_table();
    bool open_database();
    bool restore_database();
    void close_database();
    void initialize_roles();
};

#endif // DATABASE_H

#include "database.h"

DataBase::DataBase(QObject* parent): QObject{parent}
{
    initialize_roles();
    tables =
    {
        {Table::Users, USERS},
        {Table::Games, GAMES},
        {Table::Game_Sessions, GAME_SESSIONS},
        {Table::Roles, ROLES}
    };
    queries_create_table =
    {
        {
             Table::Users, "CREATE TABLE " USERS " ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             USERS_NAME " VARCHAR(255) NOT NULL, "
             USERS_PASSWORD " VARCHAR(255) NOT NULL, "
             USERS_EMAIL " VARCHAR(255) NOT NULL, "
             USERS_DATA_ON_BIRTH " DATE NOT NULL, "
             USERS_ROLE "INTEGER NOT NULL, "
             USERS_ONLINE "BOOLEAN DEFAULT FALSE)"
        },
        {
             Table::Games, "CREATE TABLE " GAMES " ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             GAMES_NAME " VARCHAR(255) NOT NULL, "
             GAMES_DESCRIPTION " VARCHAR(255) NOT NULL)"
        },
        {
            Table::Game_Sessions, "CREATE TABLE " GAME_SESSIONS " ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             GAME_SESSIONS_TIMESTAMP " DATETIME NOT NULL, "
             GAME_SESSIONS_USER_ID " INTEGER NOT NULL, "
             GAME_SESSIONS_GAME_ID " INTEGER NOT NULL, "
             GAME_SESSIONS_BET_AMOUNT " INTEGER NOT NULL, "
             GAME_SESSIONS_WIN_LOSS " INTEGER NOT NULL, "
             "FOREIGN KEY (" GAME_SESSIONS_USER_ID ") REFERENCES " USERS " (id), "
             "FOREIGN KEY (" GAME_SESSIONS_GAME_ID ") REFERENCES " GAMES " (id))"
        },
        {
            Table::Roles, "CREATE TABLE " ROLES " ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            ROLES_NAME " VARCHAR(255) UNIQUE NOT NULL)"
        }
    };
    queries_insert_into_table =
    {
        {
            Table::Users, "INSERT INTO " USERS " (" USERS_NAME ", " USERS_PASSWORD ", " USERS_EMAIL ", " USERS_DATA_ON_BIRTH ", " USERS_ROLE
            ") VALUES (:Username, :Password, :Email, :Data, :Role)"
        },
        { Table::Games, "INSERT INTO " GAMES " (" GAMES_NAME", " GAMES_DESCRIPTION ") VALUES (:Game_name, :Games_description)"},
        {
            Table::Game_Sessions, "INSERT INTO " GAME_SESSIONS
            " (" GAME_SESSIONS_TIMESTAMP", " GAME_SESSIONS_USER_ID ", " GAME_SESSIONS_GAME_ID
            ", " GAME_SESSIONS_BET_AMOUNT ", " GAME_SESSIONS_WIN_LOSS ") "
            "VALUES (:Timestamp, :User_id, :Game_id, :Bet_amount, :Win_loss)"
        },
        { Table::Roles, "INSERT INTO " ROLES " (" ROLES_NAME ") VALUES (:Name)"}
    };
    queries_remove_record =
    {
        {Table::Users, "DELETE FROM " USERS " WHERE id=:ID;"},
        {Table::Games, "DELETE FROM " GAMES " WHERE id=:ID;"},
        {Table::Game_Sessions, "DELETE FROM " GAME_SESSIONS " WHERE id=:ID;"},
        {Table::Roles, "DELETE FROM " ROLES " WHERE id=:ID;"}
    };
    query_error =
    {
        {Query_Error_Type::Create_Table, "Error of create "},
        {Query_Error_Type::Insert_Into_Table,"Error insert into "},
        {Query_Error_Type::Remove_Record, "Error delete row "},
        {Query_Error_Type::Get_Data, "Error get data - "}
    };
    value_for_request =
    {
        {QVariant::Int, [](QSqlQuery& query, const QString& key, QVariant& value) {query.bindValue(":" + key, value.toInt());}},
        {QVariant::String, [](QSqlQuery& query, const QString& key, QVariant& value) {query.bindValue(":" + key, value.toString());}},
        {QVariant::Date, [](QSqlQuery& query, const QString& key, QVariant& value) {query.bindValue(":" + key, value.toDate());}},
        {QVariant::DateTime, [](QSqlQuery& query, const QString& key, QVariant& value) {query.bindValue(":" + key, value.toDateTime());}}
    };
}

QByteArray DataBase::encrypt(const QString &data)
{
    // Используем алгоритм шифрования AES-256, где число - количество байт
    // ECB разделяет данные на блоки и шифрует каждый по отдельности
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::ECB);
    // Преобразовываем данные в байты через toUtf8() и с помощью метода encode
    // и ключа кодируем данные
    QByteArray encrypted = encryption.encode(data.toUtf8(), encryption_key);
    return encrypted;
}

QString DataBase::decrypt(const QByteArray &data)
{
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::ECB);
    // На основе алгоритма шифрования мы расшифровываем данные
    QByteArray decrypted = encryption.decode(data, encryption_key);
    // Преобразовываем байты в символы и возвращаем
    return QString::fromUtf8(decrypted);
}

void DataBase::close_database()
{
    db.close();
}

void DataBase::initialize_roles()
{
    QStringList roles = { "Player", "Manager", "Security", "Admin"};
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM " ROLES " WHERE " ROLES_NAME " = :role_name");
    for (const QString& role : roles)
    {
        query.bindValue(":role_name", role);
        if (execute_query(query, "System", "Failed add roles.") && query.value(0).toInt() == 0) insert_into_table(role);
    }
}

void DataBase::connect_to_database()
{
    // Проверяем существование БД. Если есть - открываем, иначе - восстанавливаем
    if (QFile("./" DATABASE_NAME).exists())
        this->open_database();
    else
        this->restore_database();
}

bool DataBase::restore_database()
{
    if (this->open_database()) return (query_for_create_table())? true: false;
    error_handler->handle_error("System", "The database could not be restored.");
    return false;
}

bool DataBase::open_database()
{
    /*БД откроется и создатся по заданному пути, если она существует*/
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("./" DATABASE_NAME);
    if (db.open()) return true;
    error_handler->handle_error("System", "Failed to open the database.");
    return false;
}

bool DataBase::execute_query(QSqlQuery& query, const QString& username, const QString& error_message)
{
    if (query.exec() || query.next()) return true;
    error_handler->handle_error(username, error_message + " - " + query.lastError().text());
    return false;
}

bool DataBase::check_game_in_db(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Games WHERE Name = :game_name");
    query.bindValue(":game_name", name);
    if (!execute_query(query, "System", "Game already exists in the database.") && query.value(0).toInt() == 0) return false;
    return true;
}

int DataBase::get_player_id(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM Users WHERE Username = :username");
    query.bindValue(":username", username);
    if (execute_query(query, "System", query_error[Query_Error_Type::Get_Data] + "player id.")) return query.value(0).toInt();
    return -1;
}

int DataBase::get_game_id(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM Games WHERE Name = :name");
    query.bindValue(":name", name);
    if (execute_query(query, "System", query_error[Query_Error_Type::Get_Data] + "game id.")) return query.value(0).toInt();
    return -1;
}

bool DataBase::create_table(Table table, QSqlQuery& query)
{
    return execute_query(query, "System", query_error[Query_Error_Type::Create_Table] + tables[table]);
}

bool DataBase::query_for_create_table()
{
    for (auto it = queries_create_table.begin(); it != queries_create_table.end(); ++it)
    {
        QSqlQuery query;
        query.prepare((*it).second);
        if (!create_table((*it).first, query)) return false;
    }
    return true;
}

bool DataBase::insert_into_table(Table table, QSqlQuery& query)
{
    return execute_query(query, "System", query_error[Query_Error_Type::Insert_Into_Table] + tables[table]);
}

bool DataBase::insert_into_table(const QString &username, const QString &password, const QString &email, const QDate &date, const Role& role)
{
    QVariantMap data;
    data["username"] = username;
    data["password"] = encrypt(password);
    data["email"] = encrypt(email);
    data["date"] = date;
    data["role"] = static_cast<int>(role);
    return query_for_insert_into_table(Table::Users, data);
}

bool DataBase::insert_into_table(const QString& game_name, const QString& description)
{
    QVariantMap data;
    data["game_name"] = game_name;
    data["description"] = description;
    return query_for_insert_into_table(Table::Games, data);
}

bool DataBase::insert_into_table(const QString &role_name)
{
    QVariantMap data;
    data["name"] = role_name;
    return query_for_insert_into_table(Table::Roles, data);
}

bool DataBase::insert_into_table(const QString& timestamp, const int& user_id, const int& game_id, const int& bet_amount, const int& win_loss)
{
    QVariantMap data;
    data["timestamp"] = timestamp;
    data["user_id"] = user_id;
    data["game_id"] = game_id;
    data["bet_amount"] = bet_amount;
    data["win_loss"] = win_loss;
    return query_for_insert_into_table(Table::Game_Sessions, data);
}

bool DataBase::query_for_insert_into_table(Table table, const QVariantMap &data)
{
    QSqlQuery query;
    query.prepare(queries_insert_into_table[table]);
    foreach (const QString& key, data.keys())
    {
        QVariant value = data.value(key);
        auto it = value_for_request.find(value.type());
        if (it != value_for_request.end())
            it->second(query, key, value);
        else
            return false;
    }
    return insert_into_table(table, query);
}

bool DataBase::query_remove_record(Table table, const int id)
{
    QSqlQuery query;
    query.prepare(queries_remove_record[table]);
    query.bindValue(":ID", id);
    return execute_query(query, "System", query_error[Query_Error_Type::Remove_Record] + tables[table]);
}

bool DataBase::is_user_exists(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM " USERS " WHERE " USERS_NAME " = : " USERS_NAME);
    query.bindValue(":" USERS_NAME, username);
    return execute_query(query, username, "Error checking user existence.") && query.value(0).toInt() > 0;
}

bool DataBase::create_user(const QString &username, const QString &password, const QString &email, const QDate &birth_date, Role role)
{
    if (!is_user_exists(username)) return insert_into_table(username, password, email, birth_date, role);
    error_handler->handle_error(username, "User already exists.");
    return false;
}

bool DataBase::set_user_online_status(const QString &username, const bool& online)
{
    QSqlQuery query;
    query.prepare("UPDATE " USERS " SET " USERS_ONLINE " = :" USERS_ONLINE " WHERE " USERS_NAME " = :" USERS_NAME);
    query.bindValue(":" USERS_NAME, username);
    query.bindValue(":" USERS_ONLINE, online);
    return execute_query(query, username, "Error setting user online.");
}

bool DataBase::is_user_online(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT " USERS_ONLINE " FROM " USERS " WHERE " USERS_NAME " = :" USERS_NAME);
    query.bindValue(":" USERS_NAME, username);
    return execute_query(query, username, "Error checking if user is online.") && query.value(0).toInt() == 1;
}

bool DataBase::authenticate_user(const QString& username, const QByteArray& encrypted_password)
{
    QSqlQuery query;
    query.prepare("SELECT " USERS_NAME " FROM " USERS " WHERE " USERS_NAME " = :" USERS_NAME " AND " USERS_PASSWORD " = :" USERS_PASSWORD);
    query.bindValue(":" USERS_NAME, username);
    query.bindValue(":" USERS_PASSWORD, encrypted_password);
    return execute_query(query, username, "Failed login attempt: invalid credentials") && !query.value(0).toString().isEmpty();
}

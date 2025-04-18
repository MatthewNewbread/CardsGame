#include "database_model.h"

Database_model::Database_model(DataBase* db, QObject* parent): QObject(parent), db(db)
{}

QVector<QVariantMap> Database_model::get_player_data(const QString& username)
{
    QString query_string = "SELECT * FROM " USERS " WHERE " USERS_NAME " = " USERS_NAME;
    QVariantMap bindings{{USERS_NAME, username}};
    return execute_query_and_fetch(query_string, bindings);
}

QVector<QVariantMap> Database_model::get_all_records(DataBase::Table table)
{
    QString query_string = "SELECT * FROM " + db->tables[table];
    return execute_query_and_fetch(query_string);
}

bool Database_model::delete_user(int user_id)
{
    QString query_string = "DELETE FROM " USERS " WHERE id = :id";
    QVariantMap bindings{{"id", user_id}};
    QSqlQuery query;
    query.prepare(query_string);
    foreach (const QString& key, bindings.keys()) query.bindValue(":" + key, bindings.value(key));
    return db->execute_query(query, "System", "Error deleting user.");
}

bool Database_model::update_user_role(int user_id, int role_id)
{
    QString query_string = "UPDATE " USERS " SET " USERS_ROLE " = :role WHERE id = :id";
    QVariantMap bindings{{"role", role_id}, {"id", user_id}};
    QSqlQuery query;
    query.prepare(query_string);
    foreach (const QString& key, bindings.keys())  query.bindValue(":" + key, bindings.value(key));
    return db->execute_query(query, "System", "Error updating user role.");
}

int Database_model::get_role_by_username(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT " USERS_ROLE " FROM " USERS " WHERE " USERS_NAME " = :username");
    query.bindValue(":username", username);
    if (!db->execute_query(query, "System", "Error get role.")) return -1;
    return query.value(0).toInt();
}

QVector<QVariantMap> Database_model::execute_query_and_fetch(const QString& queryString, const QVariantMap& bindings)
{
    QVector<QVariantMap> results;
    QSqlQuery query;
    query.prepare(queryString);
    foreach (const QString& key, bindings.keys()) query.bindValue(":" + key, bindings.value(key));
    if (db->execute_query(query, "System", "Error executing select query"))
    {
        while (query.next())
        {
            QVariantMap row;
            for (int i = 0; i < query.record().count(); ++i) row[query.record().fieldName(i)] = query.value(i);
            results.append(row);
        }
    }
    return results;
}

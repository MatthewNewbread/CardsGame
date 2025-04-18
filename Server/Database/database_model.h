#ifndef DATABASE_MODEL_H
#define DATABASE_MODEL_H
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
#include <QVector>
#include "database.h"


class Database_model: public QObject
{
    Q_OBJECT

public:
    explicit Database_model(DataBase* db, QObject* parent = nullptr);
    QVector<QVariantMap> get_player_data(const QString& username);
    QVector<QVariantMap> get_all_records(DataBase::Table table);
    bool delete_user(int user_id);
    bool update_user_role(int user_id, int new_role);
    int get_role_by_username(const QString& username);
private:
    DataBase* db;

    QVector<QVariantMap> execute_query_and_fetch(const QString& queryString, const QVariantMap& bindings = {});
};

#endif // DATABASE_MODEL_H

#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include <QString>
#include <QMap>
#include <QDate>
#include <QThreadPool>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "auth_manager.h"
#include "lobby_manager.h"
#include "game_manager.h"
#include "database_manager.h"
#include "Database/database_model.h"


class Server: public QTcpServer
{
    Q_OBJECT
public:
    enum class Request_Type
    {
        Authenticate,
        Register,
        Create_Session,
        Join_Session,
        View_Sessions,
        Settings_Session,
        Model_Role_User,
        Model_player_data,
        Model_all_records,
        Model_delete_user,
        Model_update_user_role,
        Player_Send_Action,
        Player_Send_Bet,
        Player_Process_Action,
        Player_Process_Bet,
        Game_Players_State,
        Game_State,
        Game_Status,
        Player_Exit,
        Send_Message,
    };
    explicit Server(QObject* parent = nullptr);
    void start_server(qint16 port);
    void send_packet(QTcpSocket* socket, const QJsonObject& json_data);
protected:
    void incomingConnection(qintptr socket_descriptor) override;
private:
    QMap<QTcpSocket*, QThread*> client_threads;
    QTcpSocket* socket;
    Auth_manager* auth_manager;
    Database_manager* db_manager;
    Game_manager* game_manager;
    Lobby_manager* lobby_manager;
    QMap<Request_Type, std::function<void(const QJsonObject&, QTcpSocket*)>> request_handlers;

    void initialize_request_handlers();
private slots:
    void handle_client(QTcpSocket* socket);
    void on_client_disconnected(QTcpSocket* socket);
};

#endif // SERVER_H

#include "auth_manager.h"
#include "database_manager.h"
#include "game_manager.h"
#include "lobby_manager.h"
#include "chat_manager.h"
#include "client.h"

Client::Client(QObject* parent):QObject(parent), socket(new QTcpSocket(this)), retry_timer(new QTimer(this)), retry_attemps(0)
{
    connect(socket, &QTcpSocket::readyRead, this, &Client::on_ready_read);
    connect(socket, &QTcpSocket::disconnected, this, &Client::on_disconnected);
    connect(retry_timer, &QTimer::timeout, this, &Client::retry_connection);
    initialize_response_handlers();
}

Client::~Client()
{
    socket->close();
}

void Client::connect_to_server(const QString &host, qint16 port)
{
    socket->connectToHost(host,port);
    if (!socket->waitForConnected(3000))
    {
        emit connection_failed();
        retry_connection();
    }
}

void Client::send_request(const QByteArray& data)
{
    socket->write(data);
}

void Client::set_username(QString username)
{
    this->username = username;
}

const QString &Client::get_username() const
{
    return username;
}

void Client::on_ready_read()
{
    QByteArray raw_data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    if (!doc.isObject())
    {
        qDebug() << "Invalid JSON received";
        return;
    }
    QJsonObject json = doc.object();
    int response_type_int = json["response_type"].toInt();
    Request_Type response_type = static_cast<Request_Type>(response_type_int);
    auto handler = response_handlers.value(response_type, nullptr);
    if (handler)
        handler(json);
    else
        qDebug() << "Unknown request type received:" << response_type_int;
}

void Client::on_disconnected()
{
    qDebug() << "Disconnected from server";
    retry_connection();
}

void Client::retry_connection()
{
    if (retry_attemps < max_retry_attemps)
    {
        retry_attemps++;
        qDebug() << "Retrying connection, attempt";
        socket->abort();
        socket->connectToHost(socket->peerName(), socket->peerPort());
    }
    else
    {
        qDebug() << "Maximum retry attempts reached. Connection failed.";
        retry_timer->stop();
    }
}

Chat_manager *Client::get_chat_manager() const
{
    return chat_manager;
}

Lobby_manager *Client::get_lobby_manager() const
{
    return lobby_manager;
}

Game_manager *Client::get_game_manager() const
{
    return game_manager;
}

Database_manager *Client::get_db_manager() const
{
    return db_manager;
}

Auth_manager *Client::get_auth_manager() const
{
    return auth_manager;
}

void Client::initialize_response_handlers()
{
    response_handlers[Request_Type::Authenticate] = [this](const QJsonObject& json_data)
    {
        QString username = json_data["username"].toString();
        bool success = json_data["succes"].toBool();
        if (success)
            emit auth_manager->auth_success(username);
        else
            emit auth_manager->auth_failure();
    };
    response_handlers[Request_Type::Register] = response_handlers[Request_Type::Authenticate];
//    response_handlers[Request_Type::Create_Session] = [this](const QJsonObject&)
//    {
//        emit lobby_manager->session_created();
//    };
//    response_handlers[Request_Type::Join_Session] = [this](const QJsonObject&)
//    {
//        emit lobby_manager->session_joined();
//    };
//    response_handlers[Request_Type::View_Sessions] = [this](const QJsonObject& json_data)
//    {
//        QJsonArray lobby_info = json_data["lobby"].toArray();
//        emit lobby_manager->sessions_viewed(lobby_info);
//    };
//    response_handlers[Request_Type::Settings_Session] = [this](const QJsonObject& json_data)
//    {
//        int max_players = json_data["max_players"].toInt();
//        int min_bet = json_data["min_bet"].toInt();
//        int max_chips = json_data["max_chips"].toInt();
//        QJsonArray games_name = json_data["games_name"].toArray();
//        emit lobby_manager->server_data_received(max_players, max_chips, min_bet, games_name);
//    };
    response_handlers[Request_Type::Model_Role_User] = [this](const QJsonObject& json_data)
    {
        Database_manager::Role role = static_cast<Database_manager::Role>(json_data["role"].toInt());
        emit db_manager->user_role_received(role);
    };
    response_handlers[Request_Type::Send_Message] = [this](const QJsonObject& json_data)
    {
        QString time = json_data["time"].toString();
        QString username = json_data["username"].toString();
        QString message = json_data["message"].toString();
        emit chat_manager->messag_received(time, username, message);
    };
}

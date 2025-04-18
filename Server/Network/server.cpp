#include "server.h"

Server::Server(QObject* parent): QTcpServer(parent)
{
    initialize_request_handlers();
}

void Server::start_server(qint16 port)
{
    if (this->listen(QHostAddress::Any, port))
        qDebug() << "Server started: " << port;
    else
        qDebug() << "Server error";
}

void Server::incomingConnection(qintptr socket_descriptor)
{
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socket_descriptor)) delete socket;
    QThread* client_thread = new QThread(this);
    socket->moveToThread(client_thread);
    client_threads[socket] = client_thread;
    connect(socket, &QTcpSocket::readyRead, this, [this]() { handle_client(socket); });
    connect(socket, &QTcpSocket::disconnected, this, [this]() { on_client_disconnected(socket); });
    client_thread->start();
}

void Server::send_packet(QTcpSocket* socket, const QJsonObject& json_data)
{
    QJsonDocument doc(json_data);
    QByteArray data = doc.toJson();
    socket->write(data);
}

void Server::initialize_request_handlers()
{
    request_handlers[Request_Type::Authenticate] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        QString username = json_data["username"].toString();
        QString password = json_data["password"].toString();
        auth_manager->process_auth_request(socket, username, password);
    };
    request_handlers[Request_Type::Register] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        QString username = json_data["username"].toString();
        QString password = json_data["password"].toString();
        QString email = json_data["email"].toString();
        QDate date = QDate::fromString(json_data["date"].toString(), Qt::ISODate);
        auth_manager->process_register_request(socket, username, password, email, date);
    };
    request_handlers[Request_Type::Create_Session] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        QString username = json_data["username"].toString();
        QString game = json_data["game"].toString();
        int max_players = json_data["max_players"].toInt();
        int count_bots = json_data["count_bots"].toInt();
        int min_bet = json_data["min_bet"].toInt();
        int starting_chips = json_data["starting_chips"].toInt();
        lobby_manager->create_session(socket, username, game, max_players, count_bots, min_bet, starting_chips);
    };
    request_handlers[Request_Type::Join_Session] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        QString username = json_data["username"].toString();
        int id = json_data["id"].toInt();
        lobby_manager->join_session(socket, username, id);
    };
    request_handlers[Request_Type::View_Sessions] = [this](const QJsonObject&, QTcpSocket* socket)
    {
        lobby_manager->view_sessions(socket);
    };
    request_handlers[Request_Type::Settings_Session] = [this](const QJsonObject&, QTcpSocket* socket)
    {
        lobby_manager->session_settings(socket);
    };
    request_handlers[Request_Type::Player_Process_Action] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        std::string action = json_data["action"].toString().toStdString();
        game_manager->process_player_action(socket, action);
    };
    request_handlers[Request_Type::Player_Process_Bet] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        int bet = json_data["bet"].toInt();
        game_manager->process_player_place_bet(socket, bet);
    };
    request_handlers[Request_Type::Model_Role_User] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        QString username = json_data["username"].toString();
        db_manager->process_user_role(socket, username);
    };
    request_handlers[Request_Type::Model_all_records] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        int name_table = json_data["name_table"].toInt();
        db_manager->process_view_table(socket, static_cast<DataBase::Table>(name_table));
    };
    request_handlers[Request_Type::Model_player_data] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        QString username = json_data["username"].toString();
        db_manager->process_view_own_records(socket, username);
    };
    request_handlers[Request_Type::Model_update_user_role] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        int user_id = json_data["user_id"].toInt();
        int role_id = json_data["role_id"].toInt();
        db_manager->process_update_user_role(socket, user_id, role_id);
    };
    request_handlers[Request_Type::Model_delete_user] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        int id_user = json_data["user_id"].toInt();
        db_manager->process_delete_user(socket, id_user);
    };
    request_handlers[Request_Type::Send_Message] = [this](const QJsonObject& json_data, QTcpSocket*)
    {
        json_data["time"] = QDateTime::currentDateTime().toString("hh:mm:ss");
        QByteArray message = QJsonDocument(json_data).toJson();
        for (QTcpSocket *client : client_threads.keys()) if (client->state() == QAbstractSocket::ConnectedState) client->write(message);
    };
    request_handlers[Request_Type::Player_Send_Action] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        std::string action = json_data["action"].toString().toStdString();
        game_manager->process_player_action(socket, action);
    };
    request_handlers[Request_Type::Player_Send_Bet] = [this](const QJsonObject& json_data, QTcpSocket* socket)
    {
        int bet = json_data["bet"].toInt();
        game_manager->process_player_place_bet(socket, bet);
    };
    request_handlers[Request_Type::Player_Exit] = [this](const QJsonObject& , QTcpSocket* socket)
    {
        game_manager->process_player_exit(socket);
    };
}

void Server::handle_client(QTcpSocket* socket)
{
    QByteArray raw_data = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw_data);
    if (!doc.isObject())
    {
        qDebug() << "Invalid JSON received";
        return;
    }
    QJsonObject json = doc.object();
    int request_type_int = json["request_type"].toInt();
    Request_Type request_type = static_cast<Request_Type>(request_type_int);
    auto handler = request_handlers.value(request_type, nullptr);
    if (handler)
        handler(json, socket);
    else
        qDebug() << "Unknown request type received:" << request_type_int;
}

void Server::on_client_disconnected(QTcpSocket* socket)
{
    QThread* client_thread = client_threads.take(socket);
    client_threads.remove(socket);
    socket->deleteLater();
    if (client_thread)
    {
        client_thread->quit();
        client_thread->wait();
        client_thread->deleteLater();
    }
}

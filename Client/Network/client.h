#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDate>

class Auth_manager;
class Database_manager;
class Game_manager;
class Lobby_manager;
class Chat_manager;

class Client: public QObject
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
    explicit Client(QObject* parent = nullptr);
    ~Client();
    void connect_to_server(const QString& host, qint16 port);
    void send_request(const QByteArray& data);
    void set_username(QString username);
    const QString& get_username() const;
    Auth_manager* get_auth_manager() const;
    Database_manager* get_db_manager() const;
    Game_manager* get_game_manager() const;
    Lobby_manager* get_lobby_manager() const;
    Chat_manager* get_chat_manager() const;
signals:
    void connection_failed();
private slots:
    void on_ready_read();
    void on_disconnected();
    void retry_connection();
private:
    static const int max_retry_attemps = 5;
    QString username;
    QTcpSocket* socket;
    QTimer* retry_timer;
    int retry_attemps;
    Auth_manager* auth_manager;
    Database_manager* db_manager;
    Game_manager* game_manager;
    Lobby_manager* lobby_manager;
    Chat_manager* chat_manager;

    QMap<Request_Type, std::function<void(const QJsonObject&)>> response_handlers;

    void initialize_response_handlers();
};

#endif // CLIENT_H

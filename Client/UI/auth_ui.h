#ifndef AUTH_UI_H
#define AUTH_UI_H
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "Network/auth_manager.h"
#include "menu.h"

class Auth_ui : public QWidget
{
    Q_OBJECT
public:
    explicit Auth_ui(Client* client, QWidget* parent = nullptr);
private slots:
    void handle_login();
    void handle_registration();
    void on_auth_success(QString username);
    void on_auth_failure();
private:
    Client* client;
    QLineEdit* username_input;
    QLineEdit* password_input;
    QPushButton* login_button;
    QPushButton* register_button;
    QLabel* status_label;

    void setup_ui();
};

#endif // AUTH_UI_H

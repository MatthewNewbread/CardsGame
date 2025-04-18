#include "auth_ui.h"
#include <QVBoxLayout>

Auth_ui::Auth_ui(Client* client, QWidget *parent): QWidget(parent), client(client)
{
    setup_ui();
    connect(client->get_auth_manager(), &Auth_manager::auth_success, this, &Auth_ui::on_auth_success);
    connect(client->get_auth_manager(), &Auth_manager::auth_failure, this, &Auth_ui::on_auth_failure);
}

void Auth_ui::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    username_input = new QLineEdit(this);
    username_input->setPlaceholderText("Enter your username");
    password_input = new QLineEdit(this);
    password_input->setPlaceholderText("Enter your password");
    password_input->setEchoMode(QLineEdit::Password);
    login_button = new QPushButton("Login", this);
    register_button = new QPushButton("Register", this);
    status_label = new QLabel(this);
    connect(login_button, &QPushButton::clicked, this, &Auth_ui::handle_login);
    connect(register_button, &QPushButton::clicked, this, &Auth_ui::handle_registration);
    layout->addWidget(username_input);
    layout->addWidget(password_input);
    layout->addWidget(login_button);
    layout->addWidget(register_button);
    layout->addWidget(status_label);
    setLayout(layout);
}

void Auth_ui::handle_login()
{
    QString username = username_input->text();
    QString password = password_input->text();
    client->get_auth_manager()->login_user(username, password);
}

void Auth_ui::handle_registration()
{
    QString username = username_input->text();
    QString password = password_input->text();
    QString email = username_input->text();
    QDate birth_date = QDate::currentDate();
    client->get_auth_manager()->registration_user(username, password, email, birth_date);
}

void Auth_ui::on_auth_success(QString username)
{
    status_label->setText("Login successful!");
    client->set_username(username);
}

void Auth_ui::on_auth_failure()
{
    status_label->setText("Login failed. Please try again.");
}

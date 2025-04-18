#include "menu.h"

Menu::Menu(Client *client, QWidget *parent): QWidget(parent), client(client)
{
//    game_ui = new Game_ui(client);
    profile_ui = new Profile_main_ui(client);
    setup_ui();
}

void Menu::setup_ui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    start_game_button = new QPushButton("Start Game", this);
    user_profile_button = new QPushButton("User Profile", this);
    exit_button = new QPushButton("Exit", this);
//    connect(start_game_button, &QPushButton::clicked, this, &Menu::launch_game);
    connect(user_profile_button, &QPushButton::clicked, this, &Menu::view_user_profile);
    connect(exit_button, &QPushButton::clicked, this, &Menu::exit_program);
    layout->addWidget(start_game_button);
    layout->addWidget(user_profile_button);
    layout->addWidget(exit_button);
    setLayout(layout);
}

//void Menu::launch_game()
//{
//    game_ui->show();
//}

void Menu::view_user_profile()
{
    profile_ui->show();
}

void Menu::exit_program()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) QApplication::quit();
}

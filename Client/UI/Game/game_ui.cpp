//#include "game_ui.h"

//Game_UI::Game_UI(Client* client, QWidget *parent): QWidget(parent), client(client)
//{
//    QVBoxLayout* main_layout = new QVBoxLayout(this);
//    chat = new Chat(new Chat_manager(client, this), this);
//    exit_button = new QPushButton(this);
//    connect(exit_button, &QPushButton::clicked, this, &Game_UI::on_exit_button_clicked);
//    main_layout->addWidget(chat);
//    main_layout->addWidget(exit_button);

//    connect(client->get_lobby_manager(), &Lobby_manager::session_created, this, &Game_UI::initialization_game);
//    connect(client->get_lobby_manager(), &Lobby_manager::session_joined, this, &Game_UI::initialization_game);
//}

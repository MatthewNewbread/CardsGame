//#include "lobby_ui.h"

//Lobby_ui::Lobby_ui(Client* client, QWidget *parent): QWidget(parent), client(client)
//{
//    QVBoxLayout* layout = new QVBoxLayout(this);
//    lobby = new QTableWidget(this);
//    lobby->setColumnCount(6);
//    lobby->setHorizontalHeaderLabels({"Game Name", "Starting Chips", "Min Bet", "Bots", "Players", "Join"});
//    lobby->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    layout->addWidget(lobby);
//    refresh_sessions(); // Первый запрос на сервер
//    refresh_timer = new QTimer(this); // Таймер для обновления списка столов каждые 10 секунд
//    connect(refresh_timer, &QTimer::timeout, this, &Lobby_ui::refresh_sessions);
//    refresh_timer->start(10000);
//    connect(client->get_lobby_manager(), &Lobby_manager::sessions_viewed, this, &Lobby_ui::fill_game_sessions);
//    create_table_button = new QPushButton("Создать собственный стол", this);
//    layout->addWidget(create_table_button);
//    connect(create_table_button, &QPushButton::clicked, this, &Lobby_ui::open_create_session_dialog);
//}

//void Lobby_ui::fill_game_sessions(const QJsonArray& sessions)
//{
//    lobby->setRowCount(0); // Очищаем таблицу перед заполнением
//    for (int i = 0; i < sessions.size(); ++i)
//    {
//        QJsonObject session = sessions[i].toObject();
//        int row = lobby->rowCount();
//        lobby->insertRow(row);
//        int maxPlayers = session["max_players"].toInt();
//        int currentPlayers = session["current_players"].toInt();
//        lobby->setItem(row, 0, new QTableWidgetItem(session["game_name"].toString()));
//        lobby->setItem(row, 1, new QTableWidgetItem(QString::number(session["starting_chips"].toInt())));
//        lobby->setItem(row, 2, new QTableWidgetItem(QString::number(session["min_bet"].toInt())));
//        lobby->setItem(row, 3, new QTableWidgetItem(QString::number(session["count_bots"].toInt())));
//        lobby->setItem(row, 4, new QTableWidgetItem(QString("%1/%2").arg(currentPlayers).arg(maxPlayers)));
//        QPushButton* connectButton = new QPushButton("Join");
//        if (currentPlayers >= maxPlayers) connectButton->setEnabled(false);
//        connect(connectButton, &QPushButton::clicked, [=]() {on_connect_button_clicked(session["id"].toInt());});
//        lobby->setCellWidget(row, 5, connectButton);
//    }
//}

//void Lobby_ui::refresh_sessions()
//{
//    client->get_lobby_manager()->view_sessions();
//}

//void Lobby_ui::on_connect_button_clicked(int id)
//{
//    client->get_lobby_manager()->join_session(client->get_username(), id);
//}

//void Lobby_ui::open_create_session_dialog()
//{
//    Create_session_ui* create_table_dialog = new Create_session_ui(client->get_lobby_manager(), this);
//    create_table_dialog->show();
//}

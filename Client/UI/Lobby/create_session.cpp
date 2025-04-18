//#include "create_session_ui.h"

//Create_session_ui::Create_session_ui(Client* client, QWidget *parent): QWidget(parent), client(client)
//{
//    setup_ui();
//    request_server_data();
//}

//void Create_session_ui::setup_ui()
//{
//    QVBoxLayout* layout = new QVBoxLayout(this);
//    max_players_input = new QSpinBox(this);
//    layout->addWidget(max_players_input);
//    min_bet_chips = new QDoubleSpinBox(this);
//    layout->addWidget(min_bet_chips);
//    chips_input = new QSpinBox(this);
//    layout->addWidget(chips_input);
//    bots_input = new QSpinBox(this);
//    layout->addWidget(bots_input);
//    game_name = new QComboBox(this);
//    layout->addWidget(game_name);
//    QPushButton* create_button = new QPushButton("Создать стол", this);
//    layout->addWidget(create_button);
//    connect(create_button, &QPushButton::clicked, this, &Create_session_ui::create_table);
//    setLayout(layout);
//}

//void Create_session_ui::request_server_data()
//{
//    connect(client->get_lobby_manager(), &Lobby_manager::server_data_received, this, &Create_session_ui::on_server_data_received);
//    client->get_lobby_manager()->get_settings();
//}

//void Create_session_ui::create_table()
//{
//    QString game_type = game_name->currentText();
//    int max_players = max_players_input->value();
//    int bots = bots_input->value();
//    int min_bet = min_bet_chips->value();
//    int chips = chips_input->value();
//    client->get_lobby_manager()->create_session(client->get_username(), game_type, max_players, bots, min_bet, chips);
//}

//void Create_session_ui::on_server_data_received(const QJsonObject& settings_session)
//{
//    game_name->clear();
//    max_players_input->setRange(1, settings_session["max_players"].toInt());
//    bots_input->setRange(0, settings_session["max_players"].toInt() - 1);
//    chips_input->setRange(0, settings_session["max_chips"].toInt());
//    min_bet_chips->setRange(settings_session["min_bet"].toInt(), settings_session["max_chips"].toInt());
//    for (const QJsonValue& value : settings_session["games_name"].toArray()) game_name->addItem(value.toString());
//}

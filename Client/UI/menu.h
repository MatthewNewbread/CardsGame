#ifndef MENU_H
#define MENU_H
#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include "user.h"
#include "Game/game_ui.h"
#include "UI/Profile/profile_main_ui.h"
#include "Network/client.h"

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(Client* client, QWidget* parent = nullptr);
private slots:
//    void launch_game();
    void view_user_profile();
    void exit_program();
private:
    Client* client;
//    Game_UI* game_ui;
    Profile_main_ui* profile_ui;

    QPushButton* start_game_button;
    QPushButton* user_profile_button;
    QPushButton* exit_button;

    void setup_ui(); // Настройка интерфейса
};

#endif // MENU_H

#ifndef PROFILE_PLAYER_UI_H
#define PROFILE_PLAYER_UI_H
#include "UI/Profile/profile_main_ui.h"

class Profile_Player_ui: public Profile_main_ui
{
public:
    explicit Profile_Player_ui(Client* client, QWidget *parent = nullptr);
protected:
    void updateData() override;
    void contextMenuEvent(QContextMenuEvent* ) override;
};

#endif // PROFILE_PLAYER_UI_H

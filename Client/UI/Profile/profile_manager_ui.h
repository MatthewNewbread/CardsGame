#ifndef PROFILE_MANAGER_UI_H
#define PROFILE_MANAGER_UI_H
#include "UI/Profile/profile_main_ui.h"

class Profile_Manager_ui: public Profile_main_ui
{
public:
    Profile_Manager_ui(Client* client, QWidget *parent = nullptr);
protected:
    void updateData() override;
    void contextMenuEvent(QContextMenuEvent* ) override;
};

#endif // PROFILE_MANAGER_UI_H

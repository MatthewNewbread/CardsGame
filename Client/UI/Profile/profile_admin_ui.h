#ifndef PROFILE_ADMIN_UI_H
#define PROFILE_ADMIN_UI_H
#include "UI/Profile/profile_main_ui.h"

class Profile_Admin_ui: public Profile_main_ui
{
public:
    Profile_Admin_ui(Client* client, QWidget *parent = nullptr);
protected:
    void updateData() override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // PROFILE_ADMIN_UI_H

#ifndef PROFILE_SECURITY_UI_H
#define PROFILE_SECURITY_UI_H
#include "UI/Profile/profile_main_ui.h"

class Profile_Security_ui: public Profile_main_ui
{
public:
    Profile_Security_ui(Client* client, QWidget *parent = nullptr);
protected:
    void updateData() override;
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // PROFILE_SECURITY_UI_H

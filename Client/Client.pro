QT += gui network \
    widgets

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Network/auth_manager.cpp \
    Network/chat_manager.cpp \
        Network/client.cpp \
    Network/database_manager.cpp \
        Network/game_manager.cpp \
    Network/lobby_manager.cpp \
    UI/Game/card.cpp \
    UI/Game/card_animation.cpp \
    UI/Game/chat.cpp \
    UI/Game/game_control.cpp \
    UI/Game/game_ui.cpp \
    UI/Game/player.cpp \
    UI/Game/table.cpp \
    UI/Lobby/create_session.cpp \
    UI/Lobby/game_session_ui.cpp \
    UI/Lobby/lobby_ui.cpp \
    UI/Profile/profile_admin_ui.cpp \
    UI/Profile/profile_main_ui.cpp \
    UI/Profile/profile_manager_ui.cpp \
    UI/Profile/profile_player_ui.cpp \
    UI/Profile/profile_security_ui.cpp \
        UI/auth_ui.cpp \
        UI/menu.cpp \
        error_handler.cpp \
        file_logger.cpp \
        main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Network/auth_manager.h \
    Network/chat_manager.h \
    Network/client.h \
    Network/database_manager.h \
    Network/game_manager.h \
    Network/lobby_manager.h \
    UI/Game/card.h \
    UI/Game/card_animation.h \
    UI/Game/chat.h \
    UI/Game/game_control.h \
    UI/Game/game_ui.h \
    UI/Game/player.h \
    UI/Game/table.h \
    UI/Lobby/create_session.h \
    UI/Lobby/game_session_ui.h \
    UI/Lobby/lobby_ui.h \
    UI/Profile/profile_admin_ui.h \
    UI/Profile/profile_main_ui.h \
    UI/Profile/profile_manager_ui.h \
    UI/Profile/profile_player_ui.h \
    UI/Profile/profile_security_ui.h \
    UI/auth_ui.h \
    UI/menu.h \
    error_handler.h \
    file_logger.h \

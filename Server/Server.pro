QT += gui sql network

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Network/database_manager.cpp \
        Network/lobby_manager.cpp \
        Network/server.cpp \
        Network/auth_manager.cpp \
        Network/game_manager.cpp \
        Database/database.cpp \
        Database/database_model.cpp \
        Database/Qt-AES-master/qaesencryption.cpp \
        Game/game.cpp \
        Game/Player/player.cpp \
        Game/Player/player_bank.cpp \
        Game/Player/player_hand.cpp \
        Game/Deck/card.cpp \
        Game/Deck/deck.cpp \
        Game/Game-Blackjack/game_blackjack.cpp \
        Game/Game-Poker/game_poker.cpp \
        lobby.cpp \
        main.cpp \
        auth.cpp \
        error_handler.cpp \
        file_logger.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Network/database_manager.h \
    Network/lobby_manager.h \
    Network/server.h \
    Network/auth_manager.h \
    Network/game_manager.h \
    Database/database.h \
    Database/database_model.h \
    Database/Qt-AES-master/qaesencryption.h \
    Game/game.h \
    Game/Player/player.h \
    Game/Player/player_bank.h \
    Game/Player/player_hand.h \
    Game/Deck/card.h \
    Game/Deck/deck.h \
    Game/Game-Blackjack/game_blackjack.h \
    Game/Game-Poker/game_poker.h \
    auth.h \
    error_handler.h \
    file_logger.h \
    lobby.h

#include <QApplication>
#include "UI/menu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Client* client = new Client();
    Menu* menu = new Menu(client);
    menu->showFullScreen();

    return app.exec();
}

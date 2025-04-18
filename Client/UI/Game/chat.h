#ifndef CHAT_H
#define CHAT_H
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include "Network/chat_manager.h"

class Chat: public QWidget
{
    Q_OBJECT
public:
    explicit Chat(Chat_manager* chat_manager ,QWidget *parent = nullptr);
    void appendMessage(const QString &time, const QString &username, const QString &message);
    void appendSystemMessage(const QString &message);
    void setUsernameColor(const QString &username, const QColor &color);  // Устанавливаем цвет юзернейма
signals:
    void messageSent(const QString &message);
private slots:
    void onSendMessage();
private:
    QTextEdit *chatBox;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    Chat_manager* chat_manager;
    QMap<QString, QColor> usernameColors;  // Карта для хранения цветов юзернеймов
    QColor generateColor();  // Генерация случайного цвета для нового юзернейма
};

#endif // CHAT_H

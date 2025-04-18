#include "chat.h"

Chat::Chat(Chat_manager* chat_manager ,QWidget *parent): QWidget(parent), chatBox(new QTextEdit(this)), messageInput(new QLineEdit(this)), sendButton(new QPushButton("Send", this)), chat_manager(chat_manager)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chatBox);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);
    chatBox->setReadOnly(true);  // Запрещаем редактировать текст чата вручную
    connect(sendButton, &QPushButton::clicked, this, &Chat::onSendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &Chat::onSendMessage);  // Отправка сообщения по Enter
    connect(this, &Chat::messageSent, chat_manager, &Chat_manager::send_message);
    connect(chat_manager, &Chat_manager::messag_received, this, &Chat::appendMessage);
}

void Chat::appendMessage(const QString &time, const QString &username, const QString &message)
{
    if (!usernameColors.contains(username)) usernameColors[username] = generateColor();
    QColor color = usernameColors.value(username);
    chatBox->setTextColor(Qt::gray);
    chatBox->append("[" + time + "] ");
    chatBox->setTextColor(color);
    chatBox->insertPlainText(username + ": ");
    chatBox->setTextColor(Qt::black);
    chatBox->insertPlainText(message);
}

void Chat::appendSystemMessage(const QString &message)
{
    chatBox->append("<i>" + message + "</i>");
}

void Chat::setUsernameColor(const QString &username, const QColor &color)
{
    usernameColors[username] = color;
}

void Chat::onSendMessage()
{
    QString message = messageInput->text().trimmed();
    if (message.isEmpty()) return;
    emit messageSent(message);  // Сигнал отправки сообщения
    messageInput->clear();  // Очищаем поле ввода после отправки
}

QColor Chat::generateColor()
{
    int red = QRandomGenerator::global()->bounded(50, 256);
    int green = QRandomGenerator::global()->bounded(50, 256);
    int blue = QRandomGenerator::global()->bounded(50, 256);
    return QColor(red, green, blue);
}

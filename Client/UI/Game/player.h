#ifndef PLAYER_H
#define PLAYER_H
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QProgressBar>
#include <QHBoxLayout>
#include "card.h"
#include"card_animation.h"

class Player: public QWidget
{
    Q_OBJECT
public:
    explicit Player(const QString &username, QWidget *parent = nullptr);
    void updatePlayerInfo(int chips, int betAmount, const QVector<Card*> &cards);
    void startTurnTimer(int duration);  // Запуск прогресс-бара для хода
signals:
    void playerTimeOut();
private:
    static const int TIME_TURN = 10;
    QLabel *usernameLabel;
    QLabel *chipsLabel;
    QLabel *betLabel;
    QProgressBar *turnProgressBar;  // Прогресс-бар для отсчета времени хода
    QVector<Card*> handCards;  // Карты на руках
    int turnDuration;  // Продолжительность хода

    void setupStyle();
    void setupBetLabel();  // Инициализация и стилизация ставки
    void updateBetPosition();  // Позиционирование ставки относительно игрока
    bool cardsChanged(const QVector<Card*> &newCards);  // Проверка, изменились ли карты
};


#endif // PLAYER_H

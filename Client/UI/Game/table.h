#ifndef GAME_SESSION_H
#define GAME_SESSION_H
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QGradient>
#include <QVector>
#include "card.h"
#include "card_animation.h"

class Table: public QWidget
{
    Q_OBJECT
public:
    explicit Table(QWidget *parent = nullptr);
    void updateTableCards(const QVector<Card*> &cards);
    void updatePot(int potAmount);
    void hideAllCards();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QVector<Card*> tableCards;
    QLabel *potLabel;
    void setupTableStyle();
    void setupPotLabel();
};

#endif // GAME_SESSION_H

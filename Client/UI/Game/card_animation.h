#ifndef CARD_ANIMATION_H
#define CARD_ANIMATION_H
#include <QPropertyAnimation>
#include <QWidget>

class Card_Animation
{
public:
    static void animateDeal(QWidget *widget, const QPoint &startPos, const QPoint &endPos, int duration = 1000);
    static void animateDiscard(QWidget *widget, int duration = 1000);
};

#endif // CARD_ANIMATION_H

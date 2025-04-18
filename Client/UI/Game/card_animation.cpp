#include "card_animation.h"

void Card_Animation::animateDeal(QWidget *widget, const QPoint &startPos, const QPoint &endPos, int duration)
{
    widget->setGeometry(startPos.x(), startPos.y(), 70, 100);  // Задаем начальную позицию для карты
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);  // Длительность анимации
    animation->setStartValue(QRect(startPos, QSize(70, 100)));  // Начальная позиция карты
    animation->setEndValue(QRect(endPos, QSize(70, 100)));  // Конечная позиция карты
    animation->start(QAbstractAnimation::DeleteWhenStopped);  // Удаляем анимацию после завершения
}

void Card_Animation::animateDiscard(QWidget *widget, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "opacity");
    animation->setDuration(duration);  // Длительность анимации
    animation->setStartValue(1.0);  // Начальная прозрачность
    animation->setEndValue(0.0);  // Полное исчезновение карты
    animation->start(QAbstractAnimation::DeleteWhenStopped);  // Удаляем анимацию после завершения
    QObject::connect(animation, &QPropertyAnimation::finished, widget, &QWidget::hide);  // Скрываем карту после анимации

}

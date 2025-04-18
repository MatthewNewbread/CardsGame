#include "card.h"

Card::Card(QWidget *parent): QWidget(parent), cardImage(new QLabel(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(cardImage);
    setLayout(layout);
}

void Card::setCard(const QString &card)
{
    path = ":/images/" + card + ".png";
    QPixmap pixmap(path);
    cardImage->setPixmap(pixmap.scaled(70, 100, Qt::KeepAspectRatio));
}

QString Card::getCardImagePath() const
{
    return path;
}

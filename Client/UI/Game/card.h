#ifndef CARD_H
#define CARD_H
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMap>
#include <functional>

class Card: public QWidget
{
    Q_OBJECT
public:
    explicit Card(QWidget *parent = nullptr);
    void setCard(const QString &card);
    QString getCardImagePath() const;
private:
    QLabel *cardImage;  // Для отображения изображения карты
    QString path;
};

#endif // CARD_H

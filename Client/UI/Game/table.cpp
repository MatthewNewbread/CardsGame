#include "table.h"

Table::Table(QWidget *parent): QWidget(parent), potLabel(new QLabel(this))
{
    setupTableStyle();
}

void Table::updateTableCards(const QVector<Card *> &cards)
{
    int cardSpacing = 100;  // Расстояние между картами
    int startPoss = (width() - (cards.size() * cardSpacing)) / 2;  // Центрируем карты
    // Добавляем только новые карты
    for (int i = 0; i < cards.size(); ++i)
    {
        Card* newCard = cards[i];
        if (!tableCards.contains(newCard))
        {
            tableCards.append(newCard);  // Добавляем только новые карты
            newCard->setParent(this);
            QPoint startPos(-100, 0);  // Анимация раздачи карт, начало вне экрана
            QPoint endPos(startPos + QPoint(startPoss + i * cardSpacing, height() / 2 - newCard->height() / 2));
            Card_Animation::animateDeal(newCard, startPos, endPos);  // Анимация раздачи
        }
    }
}

void Table::updatePot(int potAmount)
{
    potLabel->setText("Pot: " + QString::number(potAmount));   // Обновление отображения банка
}

void Table::hideAllCards()
{
    // Скрываем и удаляем все карты с анимацией
    for (Card* card : tableCards)
    {
        Card_Animation::animateDiscard(card);  // Анимация исчезновения
        card->hide();
        card->deleteLater();  // Удаляем карты после анимации
    }
    tableCards.clear();  // Очищаем список карт на столе
}

void Table::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // Рисуем стол с градиентом
    QPainter painter(this);
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, QColor(0, 128, 0));  // Зеленый
    gradient.setColorAt(0.5, QColor(0, 100, 0));  // Темно-зеленый
    gradient.setColorAt(1.0, QColor(0, 128, 0));  // Зеленый
    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    QRectF tableRect(0, 0, width(), height());
    painter.drawRoundedRect(tableRect, 50, 50);  // Максимальное скругление углов
}

void Table::setupTableStyle()
{
    setFixedSize(600, 300);  // Примерный размер стола
    move(parentWidget()->width() / 2 - width() / 2, parentWidget()->height() / 2 - height() / 2);  // Центрируем стол
}

void Table::setupPotLabel()
{
    potLabel->setText("Pot: 0");
    potLabel->setAlignment(Qt::AlignCenter);
    potLabel->setStyleSheet("font-size: 18px; color: white; background-color: rgba(0, 0, 0, 0.2); padding: 5px;");
    potLabel->setFixedSize(150, 50);  // Размер для отображения банка
    potLabel->move(width() / 2 - potLabel->width() / 2, height() / 2 - potLabel->height() - 20);  // Позиция чуть выше центра стола
}

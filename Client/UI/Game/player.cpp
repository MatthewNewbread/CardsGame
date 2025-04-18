#include "player.h"

Player::Player(const QString &username, QWidget *parent): QWidget(parent), turnProgressBar(new QProgressBar(this)), turnDuration(TIME_TURN)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // Верхняя часть: черный фон, белый текст для юзернейма
    usernameLabel = new QLabel(username, this);
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setStyleSheet("background-color: #1a1a1a; color: white; padding: 10px; font-size: 16px;");
    mainLayout->addWidget(usernameLabel);
    // Нижняя часть: светлее фон для фишек
    chipsLabel = new QLabel("Chips: 0", this);
    chipsLabel->setAlignment(Qt::AlignCenter);
    chipsLabel->setStyleSheet("background-color: #333333; color: white; padding: 10px; font-size: 14px;");
    mainLayout->addWidget(chipsLabel);
    // Добавляем прогресс-бар (для отсчета времени хода)
    turnProgressBar->setRange(0, 100);
    turnProgressBar->setValue(100);  // Начинаем с полного бара
    turnProgressBar->setStyleSheet("QProgressBar { border: 2px solid grey; border-radius: 5px; }"
                                   "QProgressBar::chunk { background-color: #00ff00; }");  // Зеленый цвет прогресса
    mainLayout->addWidget(turnProgressBar);
    setLayout(mainLayout);
    setupStyle();  // Применяем стили для игрока
}

void Player::updatePlayerInfo(int chips, int betAmount, const QVector<Card *> &cards)
{
    chipsLabel->setText("Chips: " + QString::number(chips));
    // Обновление ставки: отображаем или скрываем ставку
    if (betAmount > 0)
    {
        betLabel->setText("Bet: " + QString::number(betAmount));
        betLabel->setVisible(true);
        updateBetPosition();  // Обновляем положение ставки
    }
    else
    {
        betLabel->setVisible(false);
    }
    if (!cardsChanged(cards)) return;                                   // Проверяем, изменились ли карты
    handCards.clear();
    int cardPosition = 0;
    for (Card* card : cards)                                            // Анимация раздачи новых карт
    {
        handCards.append(card);
        // Задаем начальную и конечную позиции для анимации раздачи
        QPoint startPos(-100, 0);                                       // Начало из-за экрана
        QPoint endPos(20 + cardPosition, 100);                          // Конечная позиция на столе
        Card_Animation::animateDeal(card, startPos, endPos);
        cardPosition += 80;                                             // Смещение для каждой следующей карты
    }
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    for (Card* card : handCards) cardsLayout->addWidget(card);          // Отображаем новые карты
    if (layout()->itemAt(2))                                            // Удаляем предыдущий лейаут карт, если он существует
    {
        QLayoutItem* oldCardsLayout = layout()->takeAt(2);
        delete oldCardsLayout->layout();
    }
    layout()->addItem(cardsLayout);                                     // Добавляем новый горизонтальный лейаут с картами
}

void Player::startTurnTimer(int duration)
{
    turnDuration = duration;
    turnProgressBar->setValue(100);  // Начало с 100%
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        int progress = turnProgressBar->value() - (100 / turnDuration);
        turnProgressBar->setValue(progress > 0 ? progress : 0);
        if (turnProgressBar->value() == 0)
        {
            timer->stop();
            emit playerTimeOut();
        }
    });
    timer->start(1000);  // Таймер обновляется каждую секунду
}

void Player::setupStyle()
{
    setStyleSheet("background-color: #2e2e2e; border-radius: 15px;");  // Скругленные углы для игрока
    setFixedSize(150, 200);  // Фиксированный размер прямоугольника
}

void Player::setupBetLabel()
{
    betLabel->setAlignment(Qt::AlignCenter);
    betLabel->setStyleSheet("background-color: white; color: black; border-radius: 10px; padding: 5px;");
    betLabel->setVisible(false);  // Скрываем по умолчанию
    betLabel->setFixedSize(80, 40);
}

void Player::updateBetPosition()
{
    betLabel->move(width() / 2 - betLabel->width() / 2, -betLabel->height());
}

bool Player::cardsChanged(const QVector<Card *> &newCards)
{
    if (handCards.size() != newCards.size()) return true;
    for (int i = 0; i < handCards.size(); ++i) if (handCards[i]->getCardImagePath() != newCards[i]->getCardImagePath()) return true;  // Если изменился хотя бы один элемент
    return false;
}

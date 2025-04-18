#ifndef DECK_H
#define DECK_H
#include <vector>
#include <QRandomGenerator>
#include "card.h"

class Deck
{
private:
    std::vector<Card> deck;
    static int card_index;
public:
    Deck();
    void refresh();             // Обновление колоды
    void shuffle();             // Перетасовка колоды
    void cutoff();              // Убирает рандомное кол-во карт в колоде до 4 оставшихся карт в колоде
    void preparation();         // Подготавливает колоду карт к игре
    const Card& deal_card();    // Переход к другой карте колоды
    int size() const;           // Возвращает кол-во карт в колоде
};

#endif // DECK_H

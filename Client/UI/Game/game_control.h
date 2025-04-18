#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QMap>

class Game_Control: public QWidget
{
    Q_OBJECT
public:
    explicit Game_Control(QWidget *parent = nullptr);
    void addActionButton(const QString &actionName);  // Добавляем кнопку действия
    void addActionSlider(const QString &sliderName, int minValue, int maxValue);  // Добавляем ползунок действия
signals:
    void actionSelected(const QString &action);
    void betChanged(int amount);
private:
    QVBoxLayout *layout;  // Основной лейаут для элементов
    QMap<QString, QPushButton*> actionButtons;  // Карта для хранения кнопок
    QMap<QString, QSlider*> actionSliders;  // Карта для хранения ползунков
};

#endif // GAME_CONTROL_H

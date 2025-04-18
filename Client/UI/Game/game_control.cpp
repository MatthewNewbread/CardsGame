#include "game_control.h"

Game_Control::Game_Control(QWidget *parent): QWidget(parent), layout(new QVBoxLayout(this))
{
    setLayout(layout); // Устанавливаем основной лейаут
}

void Game_Control::addActionButton(const QString &actionName)
{
    QPushButton *button = new QPushButton(actionName, this); // Создаем новую кнопку действия и добавляем ее в лейаут
    layout->addWidget(button);
    actionButtons[actionName] = button; // Сохраняем кнопку в карту и связываем сигнал
    connect(button, &QPushButton::clicked, this, [this, actionName]() { emit actionSelected(actionName);}); // Отправляем сигнал с именем действия
}


void Game_Control::addActionSlider(const QString &sliderName, int minValue, int maxValue)
{
    QSlider *slider = new QSlider(Qt::Horizontal, this); // Создаем новый ползунок для ставок и добавляем его в лейаут
    slider->setRange(minValue, maxValue);
    layout->addWidget(slider);
    actionSliders[sliderName] = slider; // Сохраняем ползунок в карту и связываем сигнал изменения
    connect(slider, &QSlider::valueChanged, this, [this](int value) { emit betChanged(value);});  // Отправляем сигнал с измененной ставкой
}


#include "playerCount.h"

PlayerCount::PlayerCount(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

PlayerCount::~PlayerCount()
{
}

void PlayerCount::setupUI()
{
    layout = new QVBoxLayout(this);

    setStyleSheet(
        "PlayerCount {"
        " background-color: #f8f9fa;"
        "background-image: url(back.jpg)"
        "}"
        );



    nextButton = new QPushButton("Далее", this);
    QString buttonStyle =
        "QPushButton {"
        " font-family: 'Courier New', monospace;"
        " font-size: 30px;"
        " font-weight: bold;"
        " color: #000000;"
        " background-color: transparent;"
        " border: none;"
        " padding: 15px 20px;"
        " text-align: center;"
        " min-height: 30px;"
        "}"
        "QPushButton:hover {"
        " color: #A1E0FF;"
        " background-color: #3A5562;"
        "}"
        "QPushButton:pressed {"
        " color: #ffffff;"
        " background-color: #07A2F0;"
        "}";

    nextButton->setStyleSheet(buttonStyle);



    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(2);
    slider->setMaximum(10);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(1);
    slider->setValue(2);
    slider->setStyleSheet(
        "QSlider {"
        "    background-color: transparent;"
        "    min-height: 40px;"
        "}"
        "QSlider::groove:horizontal {"
        "    background-color: #0f1419;"
        "    border: 2px solid #00ff41;"
        "    height: 12px;"
        "    border-radius: 0px;"
        "    margin: 0px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background-color: #00ff41;"
        "    border: none;"
        "    height: 12px;"
        "    border-radius: 0px;"
        "}"
        "QSlider::add-page:horizontal {"
        "    background-color: #0f1419;"
        "    border: none;"
        "    height: 12px;"
        "    border-radius: 0px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background-color: #00ff41;"
        "    border: 2px solid #ffffff;"
        "    width: 20px;"
        "    height: 20px;"
        "    margin: -6px 0px;"
        "    border-radius: 0px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "    background-color: #00cc33;"
        "    border: 2px solid #00ff41;"
        "}"
        "QSlider::handle:horizontal:pressed {"
        "    background-color: #ffffff;"
        "    border: 2px solid #00ff41;"
        "}"
        "QSlider::tick:horizontal {"
        "    background-color: #00ff41;"
        "    width: 2px;"
        "    height: 8px;"
        "}"
        );



    PlayerValueLabel= new QLabel("Игроки: 2", this);
    PlayerValueLabel->setAlignment(Qt::AlignCenter);
    PlayerValueLabel->setStyleSheet(
        "QLabel {"
        " font-family: 'Courier New', monospace;"
        " font-size: 40px;"
        " font-weight: bold;"
        " color: #000000;"                // Черный текст
        " background-color: transparent;" // Прозрачный фон, убирает черный квадрат
        " border: none;"                  // Без рамки
        " padding: 15px;"
        " min-height: 30px;"
        "}"
        );





    layout->addWidget(PlayerValueLabel);
    layout->addWidget(nextButton);
    layout->addWidget(slider);
    setLayout(layout);

    setStyleSheet(
        "PlayerCount {"
        " background-color: #f8f9fa;"
        "}"
        );

    connect(slider, &QSlider::valueChanged, this, &PlayerCount::updatePlayerValueLabel);
    connect(nextButton, &QPushButton::clicked, [this]() {
        emit nextClicked(slider->value());
    });
}

void PlayerCount::updatePlayerValueLabel(int value)
{
    PlayerValueLabel->setText(QString("Игроки: %1").arg(value));
    PlayerValueLabel->setAlignment(Qt::AlignCenter);
    PlayerValueLabel->setStyleSheet(
        "QLabel {"
        " font-family: 'Courier New', monospace;"
        " font-size: 40px;"
        " font-weight: bold;"
        " color: #000000;"                // Черный текст
        " background-color: transparent;" // Прозрачный фон, убирает черный квадрат
        " border: none;"                  // Без рамки
        " padding: 15px;"
        " min-height: 30px;"
        "}"
        );

}

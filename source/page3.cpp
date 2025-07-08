#include "page3.h"

Page3::Page3(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

Page3::~Page3()
{
}

void Page3::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Заголовок "Конец игры"
    titleLabel = new QLabel("КОНЕЦ ИГРЫ", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 32px;"
        "    font-weight: bold;"
        "    color: #111111;"
        "    background-color: transparent;"
        "    padding: 40px 20px 20px 20px;"
        "    text-shadow: 2px 2px 0px #003d14;"
        "    letter-spacing: 3px;"
        "}"
        );


    // Лейбл для отображения победителя
    winnerLabel = new QLabel("Победитель будет объявлен...", this);
    winnerLabel->setAlignment(Qt::AlignCenter);
    winnerLabel->setStyleSheet(
        "QLabel {"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    color: #00ff41;"
        "    background-color: transparent;"
        "    padding: 40px 20px 20px 20px;"
        "    text-shadow: 2px 2px 0px #003d14;"
        "    letter-spacing: 3px;"
        "}"
        );

    // Создаем кнопки
    playAgainButton = new QPushButton("ИГРАТЬ СНОВА", this);
    exitButton = new QPushButton("ВЫХОД", this);

    // Стилизация кнопок
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3e8e41;"
        "}";



    playAgainButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    // Создаем layout для кнопок
    buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(playAgainButton);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(exitButton);
    buttonLayout->addStretch();

    // Добавляем элементы в главный layout
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(winnerLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // Устанавливаем фоновый цвет для всей страницы
    setStyleSheet("Page3 { background-color: #f5f5f5; }");

    setLayout(mainLayout);

    // Подключаем сигналы кнопок
    connect(playAgainButton, &QPushButton::clicked, this, &Page3::playAgainClicked);
    connect(exitButton, &QPushButton::clicked, this, &Page3::exitClicked);
}

void Page3::setWinner(const QString& winner)
{
    currentWinner = winner;
    updateDisplay();
}

void Page3::updateDisplay()
{
    if (currentWinner.isEmpty()) {
        winnerLabel->setText("Победитель будет объявлен...");
    } else {
        winnerLabel->setText(QString("🏆 ПОБЕДИТЕЛЬ: %1 🏆").arg(currentWinner));
    }
}

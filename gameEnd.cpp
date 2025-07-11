#include "gameEnd.h"

GameEnd::GameEnd(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

GameEnd::~GameEnd()
{
}

void GameEnd::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Заголовок "Конец игры"
    titleLabel = new QLabel("КОНЕЦ ИГРЫ", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 40px;"
        "    font-weight: bold;"
        "    color: #111111;"
        "    background-color: transparent;"
        "    padding: 40px 20px 20px 20px;"
        //"    text-shadow: 2px 2px 0px #003d14;"
        "    letter-spacing: 3px;"
        "}"
        );


    // Лейбл для отображения победителя
    winnerLabel = new QLabel("Победитель будет объявлен...", this);
    winnerLabel->setAlignment(Qt::AlignCenter);
    winnerLabel->setStyleSheet(
        "QLabel {"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 48px;"
        "    font-weight: bold;"
        "    color: #111111;"
        "    background-color: transparent;"
        "    padding: 40px 20px 20px 20px;"
        //"    text-shadow: 2px 2px 0px #003d14;"
        "    letter-spacing: 3px;"
        "}"
        );

    // Создаем кнопки
    playAgainButton = new QPushButton("ИГРАТЬ СНОВА", this);
    exitButton = new QPushButton("ВЫХОД", this);

    // Стилизация кнопок
    QString buttonStyle =
        "QPushButton {"
        " font-family: 'Courier New', monospace;"
        " font-size: 30px;"
        " font-weight: bold;"
        " color: #000000;"
        " background-color: transparent;"
        " border: none;"
        " padding: 15px 20px;"
        " text-align: left;"
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
    setStyleSheet("GameEnd { background-color: #f5f5f5; }");

    setLayout(mainLayout);

    // Подключаем сигналы кнопок
    connect(playAgainButton, &QPushButton::clicked, this, &GameEnd::playAgainClicked);
    connect(exitButton, &QPushButton::clicked, this, &GameEnd::exitClicked);
}

void GameEnd::setWinner(QVector<int> winners)
{
    if (winners.size() == 0)
        currentWinner = " никто ";
    else if (winners.size() == 1)
        currentWinner = "Игрок " + QString::number(winners[0] + 1);
    else {
        currentWinner = "Игроки " + QString::number(winners[0] + 1);
        for (int i = 1; i < winners.size(); i++)
            currentWinner += ", " + QString::number(winners[i] + 1);
    }
    updateDisplay();
}

void GameEnd::updateDisplay()
{
    if (currentWinner.isEmpty()) {
        winnerLabel->setText("Победитель будет объявлен...");
    } else {
        winnerLabel->setText(QString("🏆 ПОБЕДИТЕЛЬ: %1 🏆").arg(currentWinner));
    }
}

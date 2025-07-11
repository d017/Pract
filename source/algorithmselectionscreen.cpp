#include "algorithmselectionscreen.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

AlgorithmSelectionScreen::AlgorithmSelectionScreen(int playerIndex, QVector<QString> availableCodes, QWidget *parent)
    : QWidget{parent}
{
    index = playerIndex;
    codes = availableCodes;

    setStyleSheet(
        "AlgorithmSelectionScreen {"
        " background-color: #1a1a2e;"
        "}"
        );

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



    QString text = "Код игрока " + QString::number(index + 1) + ":";
    QLabel* enterLabel = new QLabel(text);
    enterLabel->setStyleSheet(
        "QLabel {"
        " font-family: 'Courier New', monospace;"
        " font-size: 24px;"
        " color: #111111;"
        " background-color: transparent;"
        " padding: 10px 20px;" // отступы, если нужны
        " letter-spacing: 2px;" // если в initial используется
        "}"
        );

    enterLabel->setAlignment(Qt::AlignCenter);
    inputEdit = new QLineEdit("");
    QPushButton* submitButton = new QPushButton("Далее");
    QPushButton* noAlgorithmButton = new QPushButton("Без алгоритма");
    QPushButton* botButton = new QPushButton("Бот");


    enterLabel->setStyleSheet(
        "QLabel {"
        " font-family: 'Courier New', monospace;"
        " font-size: 48px;"
        " font-weight: bold;"
        " color: #111111;"
        " background-color: transparent;"
        " padding: 40px 20px 20px 20px;"
        " letter-spacing: 3px;"
        "}"
        );



    submitButton->setStyleSheet(buttonStyle);
    noAlgorithmButton->setStyleSheet(buttonStyle);
    botButton->setStyleSheet(buttonStyle);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(enterLabel);
    layout->addWidget(inputEdit);
    layout->addWidget(submitButton);
    layout->addWidget(noAlgorithmButton);
    layout->addWidget(botButton);
    layout->addStretch();



    connect(botButton, &QPushButton::clicked, this, &AlgorithmSelectionScreen::onBotButtonPressed);
    connect(submitButton, &QPushButton::clicked, this, &AlgorithmSelectionScreen::onAlgorithmSubmitted);
    connect(noAlgorithmButton, &QPushButton::clicked, this, &AlgorithmSelectionScreen::onAlgorithmDenied);


    setLayout(layout);
}

void AlgorithmSelectionScreen::onBotButtonPressed() {
    emit BotSelected(index);
}

void AlgorithmSelectionScreen::onAlgorithmSubmitted() {
    if (codes.contains(inputEdit->text()))
        emit CodeSelected(index, inputEdit->text());
    else {
        QMessageBox::warning(this, "Ошибка", "Нет такого кода");
        inputEdit->setText("");
    }
}

void AlgorithmSelectionScreen::onAlgorithmDenied() {
    emit AlgorithmDenied(index);
}

AlgorithmSelectionScreen::~AlgorithmSelectionScreen() {}

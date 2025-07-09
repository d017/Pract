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

    QString text = "Код игрока " + QString::number(index + 1) + ":";
    QLabel* enterLabel = new QLabel(text);
    enterLabel->setAlignment(Qt::AlignCenter);
    inputEdit = new QLineEdit("");
    QPushButton* submitButton = new QPushButton("Далее");
    QPushButton* noAlgorithmButton = new QPushButton("Без алгоритма");
    QPushButton* botButton = new QPushButton("Бот");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(enterLabel);
    layout->addWidget(inputEdit);
    layout->addWidget(submitButton);
    layout->addWidget(noAlgorithmButton);
    layout->addWidget(botButton);

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

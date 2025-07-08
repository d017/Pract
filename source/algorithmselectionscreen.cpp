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

    QString text = "Player " + QString::number(index) + "\'s code:";
    QLabel* enterLabel = new QLabel(text);
    inputEdit = new QLineEdit("");
    QPushButton* submitButton = new QPushButton("Next");
    QPushButton* botButton = new QPushButton("Bot");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(enterLabel);
    layout->addWidget(inputEdit);
    layout->addWidget(submitButton);
    layout->addWidget(botButton);

    connect(botButton, &QPushButton::clicked, this, &AlgorithmSelectionScreen::onBotButtonPressed);
    connect(submitButton, &QPushButton::clicked, this, &AlgorithmSelectionScreen::onAlgorithmSubmitted);

    setLayout(layout);
}

void AlgorithmSelectionScreen::onBotButtonPressed() {
    emit BotSelected(index);
}

void AlgorithmSelectionScreen::onAlgorithmSubmitted() {
    if (codes.contains(inputEdit->text()))
        emit CodeSelected(index, inputEdit->text());
    else {
        QMessageBox::warning(this, "Error", "No such code");
        inputEdit->setText("");
    }
}

AlgorithmSelectionScreen::~AlgorithmSelectionScreen() {}

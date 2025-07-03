#include "StartGameScreen.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QLabel>


StartGameScreen::StartGameScreen(QWidget*) {

    QLabel* gameLabel = new QLabel("Strategy");
    QPushButton* startButton = new QPushButton("Start");
    QPushButton* loadButton = new QPushButton("Load...");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(gameLabel);
    layout->addWidget(startButton);
    layout->addWidget(loadButton);

    connect(startButton, &QPushButton::clicked, this, &StartGameScreen::onStartButtonPressed);
    connect(loadButton, &QPushButton::clicked, this, &StartGameScreen::onLoadButtonPressed);

    setLayout(layout);
}

void StartGameScreen::onStartButtonPressed() {
    emit GameInitialized();
}

void StartGameScreen::onLoadButtonPressed() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Game save file"),
        QString(),
        tr("Text Files (*.txt)")
        );
    if (!fileName.isEmpty())
        emit GameLoaded(fileName);
}

StartGameScreen::~StartGameScreen() {}

#include "mainwindow.h"

#include "initial.h"
#include "playerCount.h"
#include "algorithmselectionscreen.h"
#include "moveselectionscreen.h"
#include "gameEnd.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    currentWindow = nullptr;

    prepareGame();
}

void MainWindow::prepareGame() {
    if (currentWindow != nullptr) {
        stackedWidget->removeWidget(currentWindow);
        delete currentWindow;
        delete stackedWidget;

        delete bank;
        delete manager;
    }
    stackedWidget = new QStackedWidget;
    Initial* initScreen = new Initial;
    connect(initScreen, &Initial::startClicked, this, &MainWindow::onGameInitialized);
    currentWindow = initScreen;
    stackedWidget->addWidget(initScreen);
    stackedWidget->setCurrentIndex(0);

    setCentralWidget(stackedWidget);
    resize(800, 800);
}

void MainWindow::onGameInitialized() {

    PlayerCount* playerCountScreen = new PlayerCount;
    stackedWidget->addWidget(playerCountScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = playerCountScreen;
    connect(playerCountScreen, &PlayerCount::nextClicked, this, &MainWindow::onPlayerCountSelected);
}

void MainWindow::createAlgorithmWindow(int index) {
    QVector<QString> available_codes = {"andrew", "prokhor", "polina", "vitaliy"};

    AlgorithmSelectionScreen* algorithmSelectionScreen = new AlgorithmSelectionScreen(index, available_codes);
    stackedWidget->addWidget(algorithmSelectionScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = algorithmSelectionScreen;

    connect(algorithmSelectionScreen, &AlgorithmSelectionScreen::CodeSelected,
            this, &MainWindow::onAlgorithmSelected);
    connect(algorithmSelectionScreen, &AlgorithmSelectionScreen::BotSelected,
            this, &MainWindow::onBotSelected);
    connect(algorithmSelectionScreen, &AlgorithmSelectionScreen::AlgorithmDenied,
            this, &MainWindow::onAlgorithmDenied);
}

void MainWindow::onPlayerCountSelected(int count) {
    int GAME_LENGTH = 10; // months

    bank = new Bank(count, GAME_LENGTH);
    manager = new playerManager;

    createAlgorithmWindow(0);
}

void MainWindow::onAlgorithmSelected(int playerIndex, QString code) {
    manager->addPlayer(code);
    if (playerIndex + 1 == bank->getPlayersCount())
        startGame();
    else
        createAlgorithmWindow(playerIndex + 1);
}

void MainWindow::onBotSelected(int botIndex) {
    manager->addBot();
    if (botIndex + 1 == bank->getPlayersCount())
        startGame();
    else
        createAlgorithmWindow(botIndex + 1);
}


void MainWindow::onAlgorithmDenied(int playerIndex) {
    manager->addPlayerWithoutAlgorithm();
    if (playerIndex + 1 == bank->getPlayersCount())
        startGame();
    else
        createAlgorithmWindow(playerIndex + 1);
}


void MainWindow::startGame() {
    qDebug() << "Game started";

    startMonth();
}

void MainWindow::startMonth() {
    qDebug() << "month started";
    currentBid = bank->makeBids();

    getPlayerMove(0);
}


void MainWindow::getPlayerMove(int playerIndex) {
    qDebug() << "get " << playerIndex;
    playerProperty prop = bank->getProperty(playerIndex);
    QVector<infoTable> infoTables = bank->getInfoTable();

    bid fixedBid = currentBid;
    if (fixedBid.happyCase.index == 4 && fixedBid.happyCase.target == playerIndex)
        fixedBid.happyCase.index = 0;

    std::optional<playerMove> recommendedMove = manager->getAlgorithmMove(playerIndex, prop, infoTables, fixedBid);

    if (manager->isHuman(playerIndex) && bank->isAlive(playerIndex)) {
        createMoveWindow(playerIndex, prop, infoTables, fixedBid, recommendedMove);
    }
    else {
        if (!bank->isAlive(playerIndex))
        {
            request rb = {0, 0};
            request ps = {0, 0};
            birthdayGift g = {0, 0, 0};
            playerMove deadMove = {rb, ps, 0, 0, 0, 0, 0, g};
            playerMoves.append(deadMove);
        }
        else {
            validateMove(recommendedMove.value(), prop);
            playerMoves.append(recommendedMove.value());
        }
        if (playerIndex + 1 == bank->getPlayersCount())
            endMonth();
        else
            getPlayerMove(playerIndex + 1);
    }
}

void MainWindow::createMoveWindow(int playerIndex, playerProperty prop,
                                  QVector<infoTable> infoTables, bid bankBid,
                                  std::optional<playerMove> recommendedMove) {
    MoveSelectionScreen* moveScreen = new MoveSelectionScreen(playerIndex,
                                                              prop,
                                                              infoTables,
                                                              bankBid,
                                                              recommendedMove
                                                              );
    stackedWidget->addWidget(moveScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = moveScreen;

    connect(moveScreen, &MoveSelectionScreen::moveSelected, this, &MainWindow::onPlayerMoveSubmitted);
}

void MainWindow::onPlayerMoveSubmitted(int playerIndex, playerMove move) {
    playerMoves.append(move);
    if (playerIndex + 1 == bank->getPlayersCount())
        endMonth();
    else
        getPlayerMove(playerIndex + 1);
}

void MainWindow::endMonth() {
    qDebug() << "month ended";
    qDebug() << bank->getInfoTable().size();
    bank->getMonthResults(playerMoves);

    playerMoves.clear();

    for (int i = 0; i < bank->getPlayersCount(); i++)
        if (!bank->isAlive(i) && !deadPlayers.contains(i)) {
            deadPlayers.insert(i);
            QString deathMessage = "Игрок " + QString::number(i + 1) + " выбыл";
            QMessageBox::information(this, "Игрок выбыл", deathMessage);
        }

    if (bank->isEndGame())
        endGame();
    else
        startMonth();

}

void MainWindow::endGame() {
    qDebug() << "Points";
    QVector<int> scores = bank->calculatePoints();
    QVector<int> winners;
    int max_score = -1;
    for (int i = 0; i < scores.size(); i++) {
        if (scores[i] > max_score) {
            max_score = scores[i];
            winners.clear();
            winners.append(i);
        }
        else if (scores[i] == max_score)
            winners.append(i);
    }

    GameEnd* endScreen = new GameEnd;
    endScreen->setWinner(winners);
    stackedWidget->addWidget(endScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = endScreen;

    connect(endScreen, &GameEnd::playAgainClicked, this, &MainWindow::onPlayAgain);
    connect(endScreen, &GameEnd::exitClicked, this, &MainWindow::onExit);
}

void MainWindow::onPlayAgain() {
    prepareGame();
}

void MainWindow::onExit() {
    QCoreApplication::quit();
}

MainWindow::~MainWindow() {}


void MainWindow::validateMove(playerMove& rMove, playerProperty currProperty) {
    rMove.toProdCommon = std::min(rMove.toProdCommon, (currProperty.commonFactories - rMove.factoriesToAuto) * 2);
    rMove.toProdAuto = std::min(rMove.toProdAuto, currProperty.autoFactories * 4);

    if (rMove.buyFactories + currProperty.autoFactories + currProperty.commonFactories > 10)
        rMove.buyFactories = 10 - currProperty.autoFactories - currProperty.commonFactories;
    if (rMove.gift.raw + rMove.toProdAuto + rMove.toProdCommon > currProperty.raw) {
        int rawLeft = currProperty.raw;
        rMove.gift.raw = std::min(rawLeft, rMove.gift.raw);
        rawLeft -= rMove.gift.raw;
        if (rawLeft > 0) {
            rMove.toProdAuto = std::min(rawLeft, rMove.toProdAuto);
            rawLeft -= rMove.toProdAuto;
            if (rawLeft > 0)
                rMove.toProdCommon = std::min(rawLeft, rMove.toProdCommon);
            else
                rMove.toProdCommon = 0;
        }
        else {
            rMove.toProdAuto = 0;
            rMove.toProdCommon = 0;
        }
    }
    if (rMove.gift.prod + rMove.prodSellRequest.count > currProperty.prod) {
        int prodLeft = currProperty.prod;
        rMove.gift.prod = std::min(prodLeft, rMove.gift.prod);
        prodLeft -= rMove.gift.prod;
        if (prodLeft > 0)
            rMove.prodSellRequest.count = std::min(rMove.prodSellRequest.count, prodLeft);
        else
            rMove.prodSellRequest.count = 0;
    }
    int moneyLeft = currProperty.balance + rMove.loan;
    if (rMove.rawBuyRequest.cost != 0)
        rMove.rawBuyRequest.count = std::min(rMove.rawBuyRequest.count, moneyLeft / rMove.rawBuyRequest.cost);
    moneyLeft -= rMove.rawBuyRequest.cost * rMove.rawBuyRequest.count;
    if (moneyLeft > 0) {
        rMove.buyFactories = std::min(rMove.buyFactories, moneyLeft / 100);
        moneyLeft -= 100 * rMove.buyFactories;
        if (moneyLeft > 0)
            rMove.factoriesToAuto = std::min(rMove.factoriesToAuto, moneyLeft / 300);
        else
            rMove.factoriesToAuto = 0;
    }
}

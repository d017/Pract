#include "mainwindow.h"

#include "initial.h"
#include "rulesscreen.h"
#include "playerCount.h"
#include "algorithmselectionscreen.h"
#include "moveselectionscreen.h"
#include "gameEnd.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    currentWindow = nullptr;

    prepareGame();
}

void MainWindow::center() {

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
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
    connect(initScreen, &Initial::rulesCalled, this, &MainWindow::onRulesCalled);
    connect(initScreen, &Initial::loadClicked, this, &MainWindow::onGameLoad);

    currentWindow = initScreen;
    stackedWidget->addWidget(initScreen);
    stackedWidget->setCurrentIndex(0);

    setCentralWidget(stackedWidget);
    resize(800, 800);
}

void MainWindow::onRulesCalled() {
    RulesScreen* rulesScreen = new RulesScreen;
    rulesScreen->resize(2000, 1200);
    resize(2000, 1200);
    center();
    connect(rulesScreen, &RulesScreen::back, this, &MainWindow::onReInit);
    stackedWidget->addWidget(rulesScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = rulesScreen;
}

void MainWindow::onReInit() {
    resize(800, 800);
    center();
    Initial* initScreen = new Initial;
    connect(initScreen, &Initial::startClicked, this, &MainWindow::onGameInitialized);
    connect(initScreen, &Initial::rulesCalled, this, &MainWindow::onRulesCalled);
    connect(initScreen, &Initial::loadClicked, this, &MainWindow::onGameLoad);

    stackedWidget->addWidget(initScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = initScreen;
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
    int GAME_LENGTH = 3; // months

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
    currBids.append(currentBid);
    qDebug() << currentBid.happyCase.index << currentBid.happyCase.target;
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
                                                              recommendedMove,
                                                              currBids
                                                              );
    resize(2200, 1200);
    center();
    stackedWidget->addWidget(moveScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = moveScreen;

    connect(moveScreen, &MoveSelectionScreen::moveSelected, this, &MainWindow::onPlayerMoveSubmitted);
    connect(moveScreen, &MoveSelectionScreen::gameSaved, this, &MainWindow::onGameSaved);
}

void MainWindow::onGameSaved(QString filename) {
    qDebug() << "Game saved: " << filename;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);

    out << bank->getPlayersCount() << '\n';

    for (int i = 0; i < bank->getPlayersCount(); i++) {
        if (!manager->isHuman(i)) {
            out << "BOT";
        }
        else {
            if (manager->hasAlgorithm(i))
                out << manager->getPlayerCode(i);
            else
                out << "-";
        }
        out <<'\n';
    }

    for (int i = 0; i < bank->getPlayersCount(); i++) {
        playerProperty player = bank->getProperty(i);
        QStringList line;

        line << QString::number(player.commonFactories);
        line << QString::number(player.autoFactories);
        line << QString::number(player.balance);
        line << QString::number(player.raw);
        line << QString::number(player.prod);

        QStringList loans;

        for (int i = 0;i < player.factoryLoans.size(); i++) {
            loans << QString("F,%1,%2").arg(player.factoryLoans[i].sum).arg(player.factoryLoans[i].monthsLeft);
        }


        for (int i = 0; i < player.moneyLoans.size(); i++) {
            loans << QString("M,%1,%2").arg(player.moneyLoans[i].sum).arg(player.moneyLoans[i].monthsLeft);
        }

        line << loans.join(";");

        out << line.join(";") << "\n";
    }

    out << bank->getInfoTable().size() << '\n';
    for (const infoTable& table: bank->getInfoTable()) {
        // Write player info
        for (const playerInfo &player : table.playersTable) {
            out << "PLAYER_INFO;"
                << player.commonFactories << ";"
                << player.autoFactories << ";"
                << player.balance << ";"
                << player.raw << ";"
                << player.prod << "\n";
        }

        // Write rawSold requests
        for (const request &req : table.rawSold) {
            out << "RAW_SOLD;" << req.count << ";" << req.cost << "\n";
        }

        // Write prodBought requests
        for (const request &req : table.prodBought) {
            out << "PROD_BOUGHT;" << req.count << ";" << req.cost << "\n";
        }

        out << "TABLE_END\n";  // Mark end of one infoTable
    }

    for (const bid b : currBids) {
        // Write rawSellBid
        out << "RAW_SELL;"
            << b.rawSellBid.count << ";"
            << b.rawSellBid.cost << "\n";

        // Write prodBuyBid
        out << "PROD_BUY;"
            << b.prodBuyBid.count << ";"
            << b.prodBuyBid.cost << "\n";

        // Write happyCaseOccasion if present
        out << "HAPPY_CASE;"
            << b.happyCase.index << ";"
            << b.happyCase.target << "\n";

        out << "BID_END\n";  // End of this bid
    }

    file.close();
}

void MainWindow::onGameLoad(QString filename) {
    qDebug() << "laoding";
    try {
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return; // Error code
        }

        QTextStream in(&file);
        QString firstLine = in.readLine(); // Read the first line

        file.close();
        bool ok;
        int playerCount = firstLine.toInt(&ok);
        if (!ok)
            throw "Incorrect";
        clean();
        manager = new playerManager;


        for (int i = 0; i < playerCount; i++) {
            QString algLine = in.readLine();
            if (algLine.startsWith("BOT")) {
                manager->addBot();
            }
            else if (algLine.startsWith("-")) {
                manager->addPlayerWithoutAlgorithm();
            }
            else
                manager->addPlayer(algLine);
        }

        QVector<playerProperty> players;
        for (int i = 0; i < playerCount; i++) {
            QString line = in.readLine();

            QStringList tokens = line.split(';');
            if (tokens.size() < 6) throw "incorrect";

            playerProperty player;
            bool ok;

            player.commonFactories = tokens[0].toInt(&ok);
            if (!ok) throw "incorrect";

            player.autoFactories = tokens[1].toInt(&ok);
            if (!ok) throw "incorrect";

            player.balance = tokens[2].toInt(&ok);
            if (!ok) throw "incorrect";

            player.raw = tokens[3].toInt(&ok);
            if (!ok) throw "incorrect";

            player.prod = tokens[4].toInt(&ok);
            if (!ok) throw "incorrect";

            QStringList loanTokens = tokens.mid(5);

            for (const QString& loanStr : loanTokens) {
                QStringList parts = loanStr.split(',');
                if (parts.size() != 3) continue;

                if (parts[0] == "F") {
                    factoryPayment fp;
                    bool ok1, ok2;
                    fp.sum = parts[1].toInt(&ok1);
                    fp.monthsLeft = parts[2].toInt(&ok2);
                    if (ok1 && ok2) {
                        player.factoryLoans.append(fp);
                    }
                } else if (parts[0] == "M") {
                    moneyLoan ml;
                    bool ok1, ok2;
                    ml.sum = parts[1].toInt(&ok1);
                    ml.monthsLeft = parts[2].toInt(&ok2);
                    if (ok1 && ok2) {
                        player.moneyLoans.append(ml);
                    }
                }
            }

            players.append(player);
        }
        qDebug() << "ok";




        qDebug() << "ok";
    }
    catch (exception e) {
        QMessageBox::warning(nullptr, "Ошибка", "Неверный формат файла");
    }
}

void MainWindow::onPlayerMoveSubmitted(int playerIndex, playerMove move) {
    playerMoves.append(move);
    if (playerIndex + 1 == bank->getPlayersCount())
        endMonth();
    else
        getPlayerMove(playerIndex + 1);
}

void MainWindow::clean() {
    // if (bank != nullptr)
    //     delete bank;
    // if (manager != nullptr)
    //     delete manager;
    // currBids.clear();
    playerMoves.clear();
    deadPlayers.clear();
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
    resize(800, 800);
    center();
    currBids.clear();
    deadPlayers.clear();
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

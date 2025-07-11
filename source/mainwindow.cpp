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
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QDebug>
#include <QGuiApplication>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    currentWindow = nullptr;


    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    baseWidth = screenGeometry.width();
    baseHeight = screenGeometry.height();

    qDebug() << baseWidth << ' ' << baseHeight;
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
    resize(baseWidth * 2 / 5, baseHeight / 2);
    center();
}

void MainWindow::onRulesCalled() {
    RulesScreen* rulesScreen = new RulesScreen;
    resize(baseWidth * 4 / 5, baseHeight * 4 / 5);
    center();
    connect(rulesScreen, &RulesScreen::back, this, &MainWindow::onReInit);
    stackedWidget->addWidget(rulesScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = rulesScreen;
}

void MainWindow::onReInit() {

    Initial* initScreen = new Initial;
    connect(initScreen, &Initial::startClicked, this, &MainWindow::onGameInitialized);
    connect(initScreen, &Initial::rulesCalled, this, &MainWindow::onRulesCalled);
    connect(initScreen, &Initial::loadClicked, this, &MainWindow::onGameLoad);

    stackedWidget->addWidget(initScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = initScreen;

    resize(baseWidth * 2 / 5, baseHeight / 2);
    center();
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
    stackedWidget->addWidget(moveScreen);
    stackedWidget->setCurrentIndex(1);
    stackedWidget->removeWidget(currentWindow);
    delete currentWindow;
    currentWindow = moveScreen;

    connect(moveScreen, &MoveSelectionScreen::moveSelected, this, &MainWindow::onPlayerMoveSubmitted);
    connect(moveScreen, &MoveSelectionScreen::gameSaved, this, &MainWindow::onGameSaved);

    resize(baseWidth * 7 / 8, baseHeight * 4 / 5);
    center();
}

QString playerInfoToString(const playerInfo& info) {
    return QString("%1,%2,%3,%4,%5")
    .arg(info.commonFactories)
        .arg(info.autoFactories)
        .arg(info.balance)
        .arg(info.raw)
        .arg(info.prod);
}

// Helper function to convert a request to string
QString requestToString(const request& req) {
    return QString("%1,%2").arg(req.count).arg(req.cost);
}

// Convert an infoTable to a single string line
QString infoTableToString(const infoTable& table) {
    QStringList parts;

    // Serialize playersTable
    QStringList players;
    for (const auto& p : table.playersTable) {
        players << playerInfoToString(p);
    }
    parts << "P:" + players.join(";");

    // Serialize rawSold
    QStringList rawSold;
    for (const auto& r : table.rawSold) {
        rawSold << requestToString(r);
    }
    parts << "R:" + rawSold.join(";");

    // Serialize prodBought
    QStringList prodBought;
    for (const auto& p : table.prodBought) {
        prodBought << requestToString(p);
    }
    parts << "PB:" + prodBought.join(";");

    return parts.join("|");
}



QString happyCaseToString(const happyCaseOccasion& hc) {
    return QString("%1,%2").arg(hc.index).arg(hc.target);
}

// Convert bid to string (one line)
QString bidToString(const bid& b) {
    return QString("RawSell:%1|ProdBuy:%2|HappyCase:%3")
    .arg(requestToString(b.rawSellBid))
        .arg(requestToString(b.prodBuyBid))
        .arg(happyCaseToString(b.happyCase));
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
    for (infoTable& table: bank->getInfoTable()) {
        out << infoTableToString(table) << "\n";
    }

    for (bid& b : currBids) {
        out << bidToString(b) << "\n";
    }


    file.close();
}


playerInfo stringToPlayerInfo(const QString& str) {
    playerInfo info;
    QStringList parts = str.split(",");
    if (parts.size() == 5) {
        info.commonFactories = parts[0].toInt();
        info.autoFactories = parts[1].toInt();
        info.balance = parts[2].toInt();
        info.raw = parts[3].toInt();
        info.prod = parts[4].toInt();
    }
    return info;
}


request stringToRequest(const QString& str) {
    request req;
    QStringList parts = str.split(",");
    if (parts.size() == 2) {
        req.count = parts[0].toInt();
        req.cost = parts[1].toInt();
    }
    return req;
}

infoTable stringToInfoTable(const QString& line) {
    infoTable table;

    QStringList entries = line.split("|");
    for (const QString& entry : entries) {
        if (entry.startsWith("P:")) {
            QString data = entry.mid(2);
            QStringList players = data.split(";");
            for (const QString& p : players) {
                if (!p.isEmpty())
                    table.playersTable.append(stringToPlayerInfo(p));
            }
        } else if (entry.startsWith("R:")) {
            QString data = entry.mid(2);
            QStringList requests = data.split(";");
            for (const QString& r : requests) {
                if (!r.isEmpty())
                    table.rawSold.append(stringToRequest(r));
            }
        } else if (entry.startsWith("PB:")) {
            QString data = entry.mid(3);
            QStringList requests = data.split(";");
            for (const QString& p : requests) {
                if (!p.isEmpty())
                    table.prodBought.append(stringToRequest(p));
            }
        }
    }

    return table;
}


happyCaseOccasion stringToHappyCase(const QString& str) {
    happyCaseOccasion hc;
    QStringList parts = str.split(",");
    if (parts.size() == 2) {
        hc.index = parts[0].toInt();
        hc.target = parts[1].toInt();
    }
    return hc;
}


bid stringToBid(const QString& line) {
    bid b;

    QStringList entries = line.split("|");
    for (const QString& entry : entries) {
        if (entry.startsWith("RawSell:")) {
            b.rawSellBid = stringToRequest(entry.mid(8));
        } else if (entry.startsWith("ProdBuy:")) {
            b.prodBuyBid = stringToRequest(entry.mid(8));
        } else if (entry.startsWith("HappyCase:")) {
            b.happyCase = stringToHappyCase(entry.mid(10));
        }
    }

    return b;
}





void MainWindow::onGameLoad(QString filename) {
    qDebug() << "laoding";
    try {
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }

        QTextStream in(&file);
        QString firstLine = in.readLine();


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

        QString bcline = in.readLine();
        ok = true;
        int monthsCount = bcline.toInt(&ok);
        if (!ok)
            throw "Incorrect";

        qDebug() << monthsCount;
        QVector<infoTable> loadedInfoTables;
        for (int i = 0; i < monthsCount; i++) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                loadedInfoTables.append(stringToInfoTable(line));
            }
        }
        qDebug() << "ok";


        QVector<bid> loadedBids;

        for (int i = 0; i < monthsCount; i++) {
            QString line = in.readLine();
            loadedBids.append(stringToBid(line));
        }
        qDebug() << "ok final";
        file.close();
        int GAME_LENGTH = 36;
        bank = new Bank(playerCount, GAME_LENGTH, players, loadedInfoTables);
        this->currBids = loadedBids;
        startMonth();
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

    resize(baseWidth * 2 / 5, baseHeight / 2);
    center();
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

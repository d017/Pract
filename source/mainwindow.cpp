#include "mainwindow.h"
#include <QRandomGenerator>
#include <QApplication>
#include <vector>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createPages();
    createNavigationButtons();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create main layout
    mainLayout = new QVBoxLayout(centralWidget);

    // Create stacked widget
    stackedWidget = new QStackedWidget(this);

    // Create button layout
    buttonLayout = new QHBoxLayout();

    // Add layouts
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(stackedWidget);

    // Set window properties
    setWindowTitle("Economic Strategy");
    resize(800, 600);
}

void MainWindow::createPages()
{
    //Cоздаем начальные окна
    initialPage = new Initial(this);
    playerCount = new PlayerCount(this);
    infoTablePage = new InfoTable(this);
    bidTable = new BidTable(this);
    gameEnd = new GameEnd(this);

    // Добавляем страницы в QStackedWidget (initial будет индекс 0)
    stackedWidget->addWidget(initialPage);
    stackedWidget->addWidget(playerCount);
    stackedWidget->addWidget(infoTablePage);
    stackedWidget->addWidget(bidTable);
    stackedWidget->addWidget(gameEnd);

    // Начинаем с начального окна
    showInitialPage();

    // Подключаем сигналы Initial Page
    connect(initialPage, &Initial::startClicked, this, &MainWindow::showCountPage);
    connect(initialPage, &Initial::loadClicked, this, &MainWindow::handleLoad);

    // Подключаем сигналы остальных страниц
    connect(playerCount, &PlayerCount::nextClicked, this, &MainWindow::showInfoTable);

    connect(infoTablePage, &InfoTable::backClicked, this, &MainWindow::showCountPage);
    connect(infoTablePage, &InfoTable::nextClicked, this, &MainWindow::showBidInfoPage);

    connect(bidTable, &BidTable::backClicked, this, &MainWindow::showInfoTable);
    connect(bidTable, &BidTable::nextClicked, this, &MainWindow::showGameEndingPage);

    connect(gameEnd, &GameEnd::playAgainClicked, this, &MainWindow::showInitialPage);
    connect(gameEnd, &GameEnd::exitClicked, this, &MainWindow::handleExit);
}

void MainWindow::handleStart()
{
    // Переходим на PlayerCount для начала новой игры
    stackedWidget->setCurrentIndex(1); // PlayerCount теперь индекс 1
}

void MainWindow::handleLoad(const QString& option)
{
    qDebug() << "Загружена опция:" << option;

    // Здесь можно добавить логику загрузки в зависимости от выбранной опции
    // Например, загрузить данные из файла или сохранения

    // Для демонстрации просто переходим на PlayerCount
    stackedWidget->setCurrentIndex(1); // PlayerCount
}


void MainWindow::handlePlayAgain()
{
    // Возвращаемся к первой странице для новой игры
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::handleExit()
{
    // Закрываем приложение
    QApplication::quit();
}




void MainWindow::createNavigationButtons()
{
    // Create navigation buttons
    initialPageButton = new QPushButton("Initial", this);
    btn1 = new QPushButton("Count", this);
    InfoTableButton = new QPushButton("infoTable", this);
    btn2 = new QPushButton("BidTable", this);
    btn3 = new QPushButton("EndOfGame", this);


    // Add buttons to layout
    buttonLayout->addWidget(initialPageButton);
    buttonLayout->addWidget(btn1);
    buttonLayout->addWidget(InfoTableButton);
    buttonLayout->addWidget(btn2);
    buttonLayout->addWidget(btn3);

    // Connect signals to slots
    connect(initialPageButton, &QPushButton::clicked, this, &MainWindow::showInitialPage);
    connect(btn1, &QPushButton::clicked, this, &MainWindow::showCountPage);
    connect(InfoTableButton, &QPushButton::clicked, this, &MainWindow::showInfoTable);
    connect(btn2, &QPushButton::clicked, this, &MainWindow::showBidInfoPage);
    connect(btn3, &QPushButton::clicked, this, &MainWindow::showGameEndingPage);

}

void MainWindow::showInitialPage()
{
    stackedWidget->setCurrentIndex(0);
}
void MainWindow::showCountPage()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showInfoTable()
{
    infoTable playerData;
    int value = 5;
    for (int i = 0; i < value; ++i) {
        playerInfo player;
        player.commonFactories = (i + 1) * 2;
        player.autoFactories = i + 1;
        player.balance = (i + 1) * 1000;
        player.raw = (i + 1) * 50;
        player.production = (i + 1) * 25;

        playerData.push_back(player);
    }

    // Передаем данные в InfoTable
    infoTablePage->setPlayerData(playerData);

    // Переключаемся на InfoTable
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showBidInfoPage()
{

    // Создаем массив примеров структур bid
    std::vector<bid> bidArray;
    int value = 3;//число игроков
    // Создаем несколько примеров структур на основе полученного значения
    for (int i = 0; i < value; ++i) {
        bid exampleBid;
        exampleBid.rawSellBid.count = (i + 1) * 10;
        exampleBid.rawSellBid.cost = (i + 1) * 100;
        exampleBid.prodBuyBid.count = (i + 1) * 5;
        exampleBid.prodBuyBid.cost = (i + 1) * 150;

        // Циклично назначаем разные occasions
        switch (i % 4) {
        case 0: exampleBid.occasion = happyCaseOccasion::BIRTHDAY; break;
        case 1: exampleBid.occasion = happyCaseOccasion::ANNIVERSARY; break;
        case 2: exampleBid.occasion = happyCaseOccasion::HOLIDAY; break;
        case 3: exampleBid.occasion = happyCaseOccasion::OTHER; break;
        }

        bidArray.push_back(exampleBid);
    }

    // Передаем массив данных на вторую страницу
    bidTable->setBidData(bidArray);

    // Переключаемся на вторую страницу


    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showGameEndingPage()
{

    QStringList possibleWinners = {"Игрок 1", "Игрок 2", "Компьютер", "Команда А", "Команда Б"};
    QString winner = possibleWinners[QRandomGenerator::global()->bounded(possibleWinners.size())];

    gameEnd->setWinner(winner);

    stackedWidget->setCurrentIndex(4);
}



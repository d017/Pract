#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSet>
#include "bank.h"
#include "playermanager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGameInitialized();
    void onRulesCalled();
    void onReInit();
    void onPlayerCountSelected(int);

    void onAlgorithmSelected(int, QString);
    void onBotSelected(int);
    void onAlgorithmDenied(int);

    void onGameSaved(QString);
    void onGameLoad(QString);
    void onPlayerMoveSubmitted(int, playerMove);

    void onPlayAgain();

    void onExit();


private:
    int baseWidth;
    int baseHeight;
    void center();

    QStackedWidget *stackedWidget;
    QWidget* currentWindow;
    Bank* bank;
    playerManager* manager;
    QVector<playerMove> playerMoves;
    bid currentBid;
    QVector<bid> currBids;
    void clean();

    QSet<int> deadPlayers;

    void prepareGame();

    void createAlgorithmWindow(int);

    void startGame();

    void startMonth();

    void getPlayerMove(int);

    void createMoveWindow(int, playerProperty, QVector<infoTable>, bid, std::optional<playerMove>);

    void endMonth();

    void endGame();

    void validateMove(playerMove&, playerProperty);
};

#endif

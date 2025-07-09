#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

// Включаем отдельные классы страниц
#include "playerCount.h"
#include "BidTable.h"
#include "gameEnd.h"
#include "initial.h"
#include "infotable.h"
#include "structures.h"

#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showInitialPage();
    void showCountPage();
    void showInfoTable();
    void showBidInfoPage();
    void showGameEndingPage();

    void handlePlayAgain();     // Новый слот для кнопки "Играть снова"
    void handleExit();
    void handleStart();
    void handleLoad(const QString& option);

private:
    void setupUI();
    void createPages();
    void createNavigationButtons();

    QStackedWidget *stackedWidget;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *InfoTableButton;
    QPushButton *btn3;
    QPushButton *initialPageButton;

    // Используем отдельные классы страниц

    PlayerCount *playerCount;
    BidTable *bidTable;
    InfoTable *infoTablePage;
    GameEnd *gameEnd;
    Initial *initialPage;
};

#endif // MAINWINDOW_H

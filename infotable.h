#ifndef INFOTABLE_H
#define INFOTABLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include "structures.h"

class InfoTable : public QWidget
{
    Q_OBJECT

public:
    explicit InfoTable(QWidget *parent = nullptr, QVector<infoTable> infoTables = QVector<infoTable>(1));
    ~InfoTable();

    void setPlayerData(int month);  // Метод для установки данных

private:
    void setupUI();
    void updateDisplay();

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;
    QTableWidget *playerTable;
    QPushButton *backButton;
    QPushButton *nextButton;

    QVector<infoTable> currentInfoTables;
    infoTable currentInfoTable;
    int currentMonth;
private slots:
    void nextClicked();    // Сигнал для кнопки Next
    void backClicked();
};

#endif // INFOTABLE_H

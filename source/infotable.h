#ifndef INFOTABLE_H
#define INFOTABLE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <vector>
#include "structures.h"

class InfoTable : public QWidget
{
    Q_OBJECT

public:
    explicit InfoTable(QWidget *parent = nullptr);
    ~InfoTable();

    void setPlayerData(infoTable& playerData);  // Метод для установки данных


signals:
    void nextClicked();    // Сигнал для кнопки Next
    void backClicked();    // Сигнал для кнопки Back

private:
    void setupUI();
    void updateDisplay();

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;
    QTableWidget *playerTable;
    QPushButton *backButton;
    QPushButton *nextButton;

    infoTable currentPlayerData;
};

#endif // INFOTABLE_H

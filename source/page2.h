#ifndef PAGE2_H
#define PAGE2_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <vector>
#include "structures.h"

class Page2 : public QWidget
{
    Q_OBJECT

public:
    explicit Page2(QWidget *parent = nullptr);
    ~Page2();

    void setBidData(const std::vector<bid>& bidDataArray);

signals:
    void nextClicked();    // Сигнал для кнопки Next
    void backClicked();    // Сигнал для кнопки Back

private:
    void setupUI();
    void updateDisplay();
    QString occasionToString(happyCaseOccasion occasion);

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;
    QTableWidget *bidTable;
    QPushButton *backButton;
    QPushButton *nextButton;

    std::vector<bid> currentBidArray;
};

#endif // PAGE2_H

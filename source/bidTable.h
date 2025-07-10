#ifndef bidTable_H
#define bidTable_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QResizeEvent>
#include "structures.h"

class BidTable : public QWidget
{
    Q_OBJECT

public:
    explicit BidTable(QWidget *parent = nullptr);
    ~BidTable();

    void setBidData(const QVector<bid>& bidDataArray);

signals:
    // void nextClicked();    // Сигнал для кнопки Next
    // void backClicked();    // Сигнал для кнопки Back

private:
    void setupUI();
    void updateDisplay();
    void resizeEvent(QResizeEvent*) override;
    QString occasionToString(happyCaseOccasion occasion);

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;
    QTableWidget *bidTablewidget;
    QPushButton *backButton;
    QPushButton *nextButton;

    QVector<bid> currentBidArray;
};

#endif // bidTable_H

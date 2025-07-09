#ifndef MOVESELECTIONSCREEN_H
#define MOVESELECTIONSCREEN_H

#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>

#include "structures.h"

class MoveSelectionScreen : public QWidget
{
    Q_OBJECT
public:
    explicit MoveSelectionScreen(int, playerProperty, QVector<infoTable>, bid, std::optional<playerMove>, QWidget *parent = nullptr);
    ~MoveSelectionScreen();
signals:
    void moveSelected(int, playerMove);

private slots:
    void onProceedButtonPressed();
    void setRecommendedMove();
    void onShowBidButtonPressed();
    void onShowInfoTablesButtonPressed();
private:
    int index;
    std::optional<playerMove> recommendedMove;
    // QVector<infoTable> infoTables;
    // bid currentBankBid;
    // playerProperty property;

    QSpinBox* rawBuyCount;
    QSpinBox* rawBuyCost;
    QSpinBox* prodSellCount;
    QSpinBox* prodSellCost;

    QSpinBox* upgradeFactoriesBox;
    QSpinBox* buyFactoriesBox;
    QSpinBox* toProdCommonBox;
    QSpinBox* toProdAutoBox;

    QLineEdit* loanEdit;

    QSpinBox* rawGiftBox;
    QSpinBox* prodGiftBox;
    QSpinBox* moneyGiftBox;

    void validateRecommendedMove();
    bool isValidMove(std::optional<playerMove>);
};

#endif // MOVESELECTIONSCREEN_H

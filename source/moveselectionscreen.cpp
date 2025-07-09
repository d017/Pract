#include "moveselectionscreen.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>

MoveSelectionScreen::MoveSelectionScreen(int playerIndex,
                                         playerProperty property,
                                         QVector<infoTable> infoTables,
                                         bid bankBid,
                                         std::optional<playerMove> recommendedMove,
                                         QWidget *parent)
    : QWidget{parent}
{
    index = playerIndex;
    this->recommendedMove = recommendedMove;
    // this->property = property;
    // this->infoTables = infoTables;
    // this->currentBankBid = bankBid;

    rawBuyCount = new QSpinBox();
    //rawBuyCount->setMaximum(bankBid.rawSellBid.count);

    rawBuyCost = new QSpinBox();
    //rawBuyCost->setMaximum(property.balance);
    //rawBuyCost->setMinimum(bankBid.rawSellBid.cost);
    rawBuyCost->setSuffix("$");


    prodSellCount = new QSpinBox();
    //prodSellCount->setMaximum(bankBid.prodBuyBid.count);


    prodSellCost = new QSpinBox();
    //prodSellCost->setMaximum(bankBid.prodBuyBid.cost);
    prodSellCost->setSuffix("$");


    upgradeFactoriesBox = new QSpinBox();
    upgradeFactoriesBox->setMaximum(property.commonFactories);


    buyFactoriesBox = new QSpinBox();
    buyFactoriesBox->setMaximum(10 - property.commonFactories - property.autoFactories);


    toProdCommonBox = new QSpinBox();
    toProdCommonBox->setMaximum(property.commonFactories * 2);

    toProdAutoBox = new QSpinBox();
    toProdAutoBox->setMaximum(property.autoFactories * 4);

    loanEdit = new QLineEdit();
    loanEdit->setValidator(new QIntValidator(0, INT_MAX, this));

    QGridLayout* layout = new QGridLayout;
    QLabel* rawLabel = new QLabel("Покупка сырья");
    QLabel* prodLabel = new QLabel("Продажа продукции");

    QLabel* rawCountLabel = new QLabel("Колич.");
    QLabel* rawCostLabel = new QLabel("Цена");
    QLabel* prodCountLabel = new QLabel("Колич.");
    QLabel* prodCostLabel = new QLabel("Цена");


    layout->addWidget(rawLabel, 0, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(prodLabel, 0, 2, 1, 2, Qt::AlignCenter);


    layout->addWidget(rawCountLabel, 1, 0, Qt::AlignRight);
    layout->addWidget(rawCostLabel, 2, 0, Qt::AlignRight);
    layout->addWidget(prodCountLabel, 1, 2, Qt::AlignRight);
    layout->addWidget(prodCostLabel, 2, 2, Qt::AlignRight);


    layout->addWidget(rawBuyCount, 1, 1);
    layout->addWidget(rawBuyCost, 2, 1);
    layout->addWidget(prodSellCount, 1, 3);
    layout->addWidget(prodSellCost, 2, 3);

    QLabel* upgradeFactoriesLabel = new QLabel("Модернизация фабрик (300$/шт.)");
    QLabel* buyFactoriesLabel = new QLabel("Покупка фабрик (100$/шт.)");
    QLabel* toProdCommonLabel = new QLabel("Переработка (об. фабр.) (20$/шт.)");
    QLabel* toProdAutoLabel = new QLabel("Переработка (авт. фабр.) (8$/шт.)");

    layout->addWidget(upgradeFactoriesLabel, 3, 0, 1, 2, Qt::AlignRight);
    layout->addWidget(buyFactoriesLabel, 4, 0, 1, 2, Qt::AlignRight);
    layout->addWidget(toProdCommonLabel, 5, 0, 1, 2, Qt::AlignRight);
    layout->addWidget(toProdAutoLabel, 6, 0, 1, 2, Qt::AlignRight);
    layout->addWidget(upgradeFactoriesBox, 3, 2, 1, 2);
    layout->addWidget(buyFactoriesBox, 4, 2, 1, 2);
    layout->addWidget(toProdCommonBox, 5, 2, 1, 2);
    layout->addWidget(toProdAutoBox, 6, 2, 1, 2);

    QLabel* loanLabel = new QLabel("Кредит ($)");
    layout->addWidget(loanLabel, 7, 0, 1, 2, Qt::AlignRight);
    layout->addWidget(loanEdit, 7, 2, 1, 2, Qt::AlignLeft);

    if (bankBid.happyCase.index == 4) {

        rawGiftBox = new QSpinBox();
        rawGiftBox->setMaximum(property.raw);
        prodGiftBox = new QSpinBox();
        prodGiftBox->setMaximum(property.prod);
        moneyGiftBox = new QSpinBox();
        moneyGiftBox->setMaximum(property.balance);
        moneyGiftBox->setSuffix("$");


        QLabel* giftLabel = new QLabel("Подарок");
        QLabel* rawGiftLabel = new QLabel("Единицы сырья");
        QLabel* prodGiftLabel = new QLabel("Единицы продукции");
        QLabel* moneyGiftLabel = new QLabel("Деньги");
        layout->addWidget(giftLabel, 8, 0, 3, 1, Qt::AlignRight);
        layout->addWidget(rawGiftLabel, 8, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(prodGiftLabel, 9, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(moneyGiftLabel, 10, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(rawGiftBox, 8, 2, 1, 2);
        layout->addWidget(prodGiftBox, 9, 2, 1, 2);
        layout->addWidget(moneyGiftBox, 10, 2, 1, 2);
    }
    else {
        moneyGiftBox = nullptr;
    }

    QPushButton* procceedButton = new QPushButton("Сделать ход");
    connect(procceedButton, &QPushButton::clicked, this, &MoveSelectionScreen::onProceedButtonPressed);
    layout->addWidget(procceedButton, 11, 0, 1, 4);

    QPushButton* recommendedMoveButton = new QPushButton("Рекомендованный ход");
    connect(recommendedMoveButton, &QPushButton::clicked, this, &MoveSelectionScreen::setRecommendedMove);
    layout->addWidget(recommendedMoveButton, 12, 0, 1, 4);

    QPushButton* bidButton = new QPushButton("Посмотреть ставки банка");
    connect(bidButton, &QPushButton::clicked, this, &MoveSelectionScreen::onShowBidButtonPressed);
    layout->addWidget(bidButton, 13, 0, 1, 4);

    QPushButton* tablesButton = new QPushButton("Посмотреть таблицы информации");
    connect(tablesButton, &QPushButton::clicked, this, &MoveSelectionScreen::onShowInfoTablesButtonPressed);
    layout->addWidget(tablesButton, 14, 0, 1, 4);


    if (recommendedMove.has_value() && isValidMove(recommendedMove))
        setRecommendedMove();

    setLayout(layout);
}


void MoveSelectionScreen::onProceedButtonPressed() {
    request rawBuyRequest = {rawBuyCount->value(), rawBuyCost->value()};
    request prodSellRequest = {prodSellCount->value(), prodSellCost->value()};
    int factoriesToAuto = upgradeFactoriesBox->value();
    int buyFactories = buyFactoriesBox->value();
    int toProdCommon = toProdCommonBox->value();
    int toProdAuto = toProdAutoBox->value();
    int loan = (loanEdit->text()).toInt();
    birthdayGift gift = {0, 0, 0};

    if (moneyGiftBox != nullptr) {
        gift.raw = rawGiftBox->value();
        gift.prod = prodGiftBox->value();
        gift.money = moneyGiftBox->value();
    }

    playerMove inputMove = {rawBuyRequest, prodSellRequest, factoriesToAuto,
                        buyFactories, toProdCommon, toProdAuto, loan, gift};
    if (isValidMove(inputMove))
        emit moveSelected(index, inputMove);
    else
        QMessageBox::warning(this, "Ошибка", "Невозможный ход");

}

void MoveSelectionScreen::onShowInfoTablesButtonPressed() {
    // show infoTables
}

void MoveSelectionScreen::onShowBidButtonPressed() {
    // show bankBid
}

void MoveSelectionScreen::setRecommendedMove() {
    if (!recommendedMove.has_value()) {
        QMessageBox::information(this, "Рекомендованный ход", "Нет алгоритма");
        return;
    }
    // set move recommendedMove
    QMessageBox::information(this, "Рекомендованный ход", "Установлен рекомендованный ход");
}
\
bool MoveSelectionScreen::isValidMove(std::optional<playerMove> move) {
    if (!move.has_value())
        return true;
    //validation of parameter
    return true;
}

void MoveSelectionScreen::validateRecommendedMove() {

}

MoveSelectionScreen::~MoveSelectionScreen(){};

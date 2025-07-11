#include "moveselectionscreen.h"
#include "infotable.h"
#include "bidTable.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>
#include <QFileDialog>


MoveSelectionScreen::MoveSelectionScreen(int playerIndex,
                                         playerProperty property,
                                         QVector<infoTable> infoTables,
                                         bid bankBid,
                                         std::optional<playerMove> recommendedMove,
                                         QVector<bid> bids,
                                         QWidget *parent)
    : QWidget{parent}
{
    index = playerIndex;
    this->recommendedMove = recommendedMove;
    this->currProperty = property;
    this->curInfoTables = infoTables;
    this->currBids = bids;


    rawBuyCount = new QSpinBox();
    //rawBuyCount->setMaximum(bankBid.rawSellBid.count);

    rawBuyCost = new QSpinBox();
    rawBuyCost->setMaximum(property.balance);
    //rawBuyCost->setMinimum(bankBid.rawSellBid.cost);
    rawBuyCost->setSuffix("$");


    prodSellCount = new QSpinBox();
    prodSellCount->setMaximum(property.prod);


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

    // new
    QString labelStyle = "QLabel {"
    "    text-align: center;"
    "    font-family: serif;"
    "    font-size: 24;"
    "    font-weight: normal;"
    "    color: #111111;"
    "    background-color: transparent;"
    //"    padding: 40px 20px 20px 20px;"
    //"    text-shadow: 2px 2px 0px #003d14;"
    "    letter-spacing: 1px;"
                         "}";

    QString indexText = "Игрок " + QString::number(index + 1);
    QLabel* indexLabel = new QLabel(indexText);
    indexLabel->setStyleSheet(labelStyle);

    QString moneyText = "Деньги: " + QString::number(currProperty.balance);
    QLabel* moneyLabel = new QLabel(moneyText);
    moneyLabel->setStyleSheet(labelStyle);

    QString rawText = "Единицы сырья: " + QString::number(currProperty.raw);
    QLabel* rwLabel = new QLabel(rawText);
    rwLabel->setStyleSheet(labelStyle);

    QString prodText = "Единицы продукции: " + QString::number(currProperty.prod);
    QLabel* prdLabel = new QLabel(prodText);
    prdLabel->setStyleSheet(labelStyle);

    QString cfText = "Обычные фабрики: " + QString::number(currProperty.commonFactories);
    QLabel* cfdLabel = new QLabel(cfText);
    cfdLabel->setStyleSheet(labelStyle);

    QString afText = "Автоматические фабрики: " + QString::number(currProperty.autoFactories);
    QLabel* afLabel = new QLabel(afText);
    afLabel->setStyleSheet(labelStyle);

    layout->addWidget(indexLabel, 0, 4, 1, 4, Qt::AlignCenter);
    layout->addWidget(moneyLabel, 1, 4, 1, 4, Qt::AlignCenter);
    layout->addWidget(rwLabel, 2, 4, 1, 4, Qt::AlignCenter);
    layout->addWidget(prdLabel, 3, 4, 1, 4, Qt::AlignCenter);
    layout->addWidget(cfdLabel, 4, 4, 1, 4, Qt::AlignCenter);
    layout->addWidget(afLabel, 5, 4, 1, 4, Qt::AlignCenter);

    QPushButton* saveButton = new QPushButton("Сохранить...");

    connect(saveButton, &QPushButton::clicked, this, &MoveSelectionScreen::onSaveButtonPressed);
    layout->addWidget(saveButton, 11, 4, 2, 4);

    // QPushButton* bidButton = new QPushButton("Посмотреть ставки банка");
    // connect(bidButton, &QPushButton::clicked, this, &MoveSelectionScreen::onShowBidButtonPressed);
    // layout->addWidget(bidButton, 13, 0, 1, 4);
    InfoTable* infoScreen = new InfoTable(nullptr, curInfoTables);
    layout->addWidget(infoScreen, 13, 0, 4, 8);

    // QPushButton* tablesButton = new QPushButton("Посмотреть таблицы информации");
    // connect(tablesButton, &QPushButton::clicked, this, &MoveSelectionScreen::onShowInfoTablesButtonPressed);
    // layout->addWidget(tablesButton, 14, 0, 1, 4);
    BidTable* bidScreen = new BidTable(nullptr);
    bidScreen->setBidData(currBids);
    layout->addWidget(bidScreen, 17, 0, 4, 8);


    if (recommendedMove.has_value() && checkValidMove(recommendedMove).isEmpty())
        setRecommendedMove();
    else if (recommendedMove.has_value() && !checkValidMove(recommendedMove).isEmpty()) {
        validateRecommendedMove();
        setRecommendedMove();
    }
    setLayout(layout);
    resize(2200, 1200);
}

void MoveSelectionScreen::onSaveButtonPressed() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Text File"),
        QString(),
        tr("Text Files (*.txt);;All Files (*)")
        );
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".txt", Qt::CaseInsensitive))
        fileName += ".txt";

    // log
    emit gameSaved(fileName);

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
    if (checkValidMove(inputMove).isEmpty())
        emit moveSelected(index, inputMove);
    else
        QMessageBox::warning(this, "Ошибка", checkValidMove(inputMove));

}

void MoveSelectionScreen::onShowInfoTablesButtonPressed() {
    InfoTable* infoScreen = new InfoTable(nullptr, curInfoTables);
    infoScreen->setWindowFlags(Qt::Window);
    infoScreen->setWindowModality(Qt::WindowModal);
    infoScreen->resize(1200, 600);
    infoScreen->show();

}

void MoveSelectionScreen::onShowBidButtonPressed() {
    // show bankBid
    BidTable* bidScreen = new BidTable(nullptr);
    bidScreen->setBidData(currBids);
    bidScreen->setWindowFlags(Qt::Window);
    bidScreen->setWindowModality(Qt::WindowModal);
    bidScreen->resize(1600, 600);
    bidScreen->show();
}

void MoveSelectionScreen::setRecommendedMove() {
    if (!recommendedMove.has_value()) {
        QMessageBox::information(this, "Ход", "Нет алгоритма для рекомендации хода");
        return;
    }

    rawBuyCount->setValue(recommendedMove->rawBuyRequest.count);
    rawBuyCost->setValue(recommendedMove->rawBuyRequest.cost);

    prodSellCount->setValue(recommendedMove->prodSellRequest.count);
    prodSellCost->setValue(recommendedMove->prodSellRequest.cost);

    upgradeFactoriesBox->setValue(recommendedMove->factoriesToAuto);
    buyFactoriesBox->setValue(recommendedMove->buyFactories);

    toProdCommonBox->setValue(recommendedMove->toProdCommon);
    toProdAutoBox->setValue(recommendedMove->toProdAuto);

    loanEdit->setText(QString::number(recommendedMove->loan));

    if (moneyGiftBox != nullptr) {
        rawGiftBox->setValue(recommendedMove->gift.raw);
        prodGiftBox->setValue(recommendedMove->gift.prod);
        moneyGiftBox->setValue(recommendedMove->gift.money);
    }

    QMessageBox::information(this, "Ход", "Установлен рекомендованный ход");
}

QString MoveSelectionScreen::checkValidMove(std::optional<playerMove> move) {
    if (!move.has_value())
        return "";

    playerMove rMove = move.value();

    if (rMove.buyFactories + currProperty.autoFactories + currProperty.commonFactories > 10)
        return "Нельзя иметь больше 10 фабрик";
    if (rMove.gift.raw + rMove.toProdAuto + rMove.toProdCommon > currProperty.raw)
        return "Недостаточно сырья";
    if (rMove.gift.prod + rMove.prodSellRequest.count > currProperty.prod)
        return "Недостаточно продукции";
    if (rMove.toProdAuto > currProperty.autoFactories * 4)
        return "Недостаточно автоматических фабрик";
    if (rMove.toProdCommon > (currProperty.commonFactories - rMove.factoriesToAuto) * 2)
        return "Недостаточно обычных фабрик";

    int moneyLeft = currProperty.balance + rMove.loan;
    moneyLeft -= rMove.rawBuyRequest.cost * rMove.rawBuyRequest.count;
    moneyLeft -= rMove.buyFactories * 100;
    moneyLeft -= rMove.factoriesToAuto * 300;
    if (moneyLeft < 0)
        return "Недостаточно денег";

    return "";
}

void MoveSelectionScreen::validateRecommendedMove() {
    playerMove rMove = recommendedMove.value();
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

MoveSelectionScreen::~MoveSelectionScreen(){};

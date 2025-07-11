#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QVector>

struct factoryPayment {
    int sum;
    int monthsLeft;
};

struct moneyLoan {
    int sum;
    int monthsLeft;
};

struct playerInfo {
    int commonFactories;
    int autoFactories;
    int balance;

    int raw;
    int prod;
};


struct playerProperty: public playerInfo {
    QVector<factoryPayment> factoryLoans;
    QVector<moneyLoan> moneyLoans;
};

struct request {
    int count;
    int cost;
};

struct happyCaseOccasion {
    int index;
    int target;
};

struct bid {
    request rawSellBid;
    request prodBuyBid;

    happyCaseOccasion happyCase;
};

struct infoTable {
    QVector<playerInfo> playersTable;
    QVector<request> rawSold;
    QVector<request> prodBought;

    playerInfo operator [] (int index) const {
        return playersTable[index];
    }
};


struct birthdayGift {
    int raw;
    int prod;
    int money;
};

struct playerMove {
    request rawBuyRequest;
    request prodSellRequest;

    int factoriesToAuto;
    int buyFactories;
    int toProdCommon;
    int toProdAuto;

    int loan;
    birthdayGift gift;
};



#endif // STRUCTURES_H

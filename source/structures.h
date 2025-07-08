
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
struct request {
    int count;
    int cost;
};

enum class happyCaseOccasion {
    // Добавьте здесь нужные значения перечисления
    BIRTHDAY,
    ANNIVERSARY,
    HOLIDAY,
    OTHER
};

struct bid {
    request rawSellBid;
    request prodBuyBid;
    happyCaseOccasion occasion;
};


struct playerInfo {
    int commonFactories;
    int autoFactories;
    int balance;
    int raw;
    int production;
};

typedef std::vector<playerInfo> infoTable;

#endif // STRUCTURES_H

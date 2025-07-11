
#include "polina.h"
// Конструктор по умолчанию не надо наверное 
Polina::Polina() {}

int Polina::calculateSafeBalance(const playerProperty& properties) {
    double loanRiskMultiplier = 1.0;
    double factoryRiskMultiplier = 1.0;

    //  Увеличение рискового коэффициента в зависимости от срока платежей
    for (const auto& loan : properties.moneyLoans) {
        if (loan.monthsLeft <= 3) {
            loanRiskMultiplier += (4 - loan.monthsLeft) * 0.25;
        }
    }

    for (const auto& fp : properties.factoryLoans) {
        if (fp.monthsLeft <= 3) {
            factoryRiskMultiplier += (4 - fp.monthsLeft) * 0.25;
        }
    }

    //  Прогнозируемые выплаты с учётом риска
    int factoryPay = 0;
    for (const auto& fp : properties.factoryLoans) {
        factoryPay += static_cast<int>(fp.sum * factoryRiskMultiplier / fp.monthsLeft);
    }

    int loanPay = 0;
    for (const auto& loan : properties.moneyLoans) {
        loanPay += static_cast<int>(loan.sum * 1.1 * loanRiskMultiplier / loan.monthsLeft);
    }

    //  Налог на фабрики в каждый 12-й месяц
    int factoryTax = 0;
    if (currentMonth % 12 == 0) {
        factoryTax += properties.commonFactories * 50;
        factoryTax += properties.autoFactories * 100;
    }

    //  Налог на ресурсы
    int resourceTax = properties.raw * 2 + properties.prod * 10;

    //  Буфер безопасности — увеличивается перед критическими месяцами
    int buffer = (currentMonth % 12 == 11 || currentMonth % 12 == 0) ? 300 : 100;

    return factoryPay + loanPay + factoryTax + resourceTax + buffer;
}


playerMove Polina::getMove(playerProperty properties, QVector<infoTable> info, bid currentBid) {
    playerMove move;

    // Счастливый случай
    if (currentBid.happyCase.index == 4) {
        move.gift.money = std::min(100, properties.balance);
        move.gift.raw = (properties.raw > 0) ? 1 : 0;
        move.gift.prod = (properties.prod > 0 && properties.raw == 0) ? 1 : 0;
    }

    // Прогноз цен
    double avgRawSell = 0, avgProdBuy = 0;
    int totalRaw = 0, totalProd = 0;
    for (const auto& t : info) {
        for (const auto& r : t.rawSold) {
            avgRawSell += r.cost * r.count;
            totalRaw += r.count;
        }
        for (const auto& p : t.prodBought) {
            avgProdBuy += p.cost * p.count;
            totalProd += p.count;
        }
    }
    double predictedRawBuyPrice = (totalRaw > 0) ? avgRawSell / totalRaw : currentBid.rawSellBid.cost;
    double predictedProdSellPrice = (totalProd > 0) ? avgProdBuy / totalProd : currentBid.prodBuyBid.cost;

    // Производственные потребности
    int requiredRaw = properties.commonFactories * 2 + properties.autoFactories * 4;
    int rawToBuy = std::max(0, requiredRaw - properties.raw);

    // Складской анализ
    bool prodOverflow = properties.prod > 8;
    bool rawDeficit = properties.raw < requiredRaw - 3;
    bool rawEmpty = properties.raw == 0;

    //  Анализ запасов всех игроков(ГП и ЕС)
    int totalCompetitorRaw = 0, totalCompetitorProd = 0;
    int playerCount = info.back().playersTable.size();
    for (const auto& p : info.back().playersTable) {
        totalCompetitorRaw += p.raw;
        totalCompetitorProd += p.prod;
    }
    int avgCompetitorRaw = (playerCount > 0) ? totalCompetitorRaw / playerCount : 0;
    int avgCompetitorProd = (playerCount > 0) ? totalCompetitorProd / playerCount : 0;

    // Продажа продукции — корректировка по рынку
    int prodSellLimit = std::min(properties.prod, currentBid.prodBuyBid.count);
    int baseProdSellPrice = prodOverflow
        ? std::max(30, currentBid.prodBuyBid.cost - 3)
        : std::max(45, std::min((int)(predictedProdSellPrice - 1), currentBid.prodBuyBid.cost - 1));

    if (avgCompetitorProd > 6) baseProdSellPrice -= 1;
    else if (avgCompetitorProd < 3) baseProdSellPrice += 1;

    move.prodSellRequest.count = prodSellLimit;
    move.prodSellRequest.cost = baseProdSellPrice;

    // Покупка сырья — корректировка по рынку
    int rawBuyLimit = std::min(rawToBuy, currentBid.rawSellBid.count);
    int baseRawBuyPrice = rawDeficit || rawEmpty
        ? std::max(currentBid.rawSellBid.cost + 2, (int)(predictedRawBuyPrice + 2))
        : std::max(currentBid.rawSellBid.cost + 1, (int)(predictedRawBuyPrice + 1));

    if (avgCompetitorRaw < 3) baseRawBuyPrice += 1;

    move.rawBuyRequest.count = rawBuyLimit;
    move.rawBuyRequest.cost = baseRawBuyPrice;

    // Фабрики конкурентов
    int totalCompetitorFactories = 0;
    for (const auto& p : info.back().playersTable) {
        totalCompetitorFactories += p.commonFactories + p.autoFactories;
    }

    bool wantsFactory = (properties.commonFactories + properties.autoFactories < 5);
    bool wantsUpgrade = (properties.commonFactories > 0);

    if (totalCompetitorFactories > playerCount * 4) {
        move.buyFactories = 1;
        move.factoriesToAuto = std::min(2, properties.commonFactories);
    }
    else {
        move.buyFactories = wantsFactory ? 1 : 0;
        move.factoriesToAuto = wantsUpgrade ? 1 : 0;
    }

    // Производство
    int rawCommon = std::min(properties.raw, properties.commonFactories * 2);
    move.toProdCommon = rawCommon;
    int rawAuto = std::min(properties.raw - rawCommon, properties.autoFactories * 4);
    move.toProdAuto = rawAuto;

    // Финансы
    int rawCost = move.rawBuyRequest.count * move.rawBuyRequest.cost;
    int factoryCost = wantsFactory ? 100 : 0;
    int upgradeCost = wantsUpgrade ? 300 : 0;
    int expectedBalance = properties.balance
        + move.prodSellRequest.count * move.prodSellRequest.cost
        - rawCost - factoryCost - upgradeCost;

    int safeBalance = calculateSafeBalance(properties);
    int loanGap = safeBalance - expectedBalance + 50;
    move.loan = (loanGap > 0) ? std::min(loanGap, 1000) : 0;

    // Следующий месяц
    this->nextMonth();
    return move;
}



// Реализация метода getCode
QString Polina::getCode() {
    return "vitaliy";
}

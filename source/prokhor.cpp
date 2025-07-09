#include <iostream>
#include "structures.h"
#include "playeralgorithm.h"

using namespace std;

class Prokhor : public playerAlgorithm {
    playerMove getMove(playerProperty pp, QVector<infoTable> it, bid b) {
        playerMove move;
        move.rawBuyRequest = { 0, 0 };
        move.prodSellRequest = { 0, 0 };
        move.factoriesToAuto = 0;
        move.buyFactories = 0;
        move.toProdCommon = 0;
        move.toProdAuto = 0;
        move.loan = 0;
        move.gift = { 0, 0, 0 };

        int currentTurn = it.size();
        int turnsLeft = 36 - currentTurn;

        // Подсчёт долгов
        int totalLoans = 0;
        for (auto& loan : pp.moneyLoans) totalLoans += loan.sum;
        for (auto& fl : pp.factoryLoans) totalLoans += fl.sum;

        // День рождения
        if (b.happyCase.index == 4) {
            if (turnsLeft > 6) {
                if (pp.raw > 3 * (2 * pp.commonFactories + 4 * pp.autoFactories)) {
                    move.gift.raw = pp.raw - 3 * (2 * pp.commonFactories + 4 * pp.autoFactories);
                }
                else if (pp.prod > 3 * (2 * pp.commonFactories + 4 * pp.autoFactories)) {
                    move.gift.prod = pp.prod - 3 * (2 * pp.commonFactories + 4 * pp.autoFactories);
                }
                else {
                    move.gift.raw = 1;
                }
            }
            else {
                move.gift.raw = 1;
            }
        }

        // Покупка фабрик
        if (pp.prod < 8 && turnsLeft > 12 && (pp.balance - totalLoans) > 300) {
            move.buyFactories = 1;
        }
        else move.buyFactories = 0;

        // Модернизация
        if (turnsLeft > 13 && (pp.balance - totalLoans) > 1000) {
            move.factoriesToAuto = 1;
        }
        else move.factoriesToAuto = 0;

        // Переработка
        int maxProcess = pp.autoFactories * 4 + pp.commonFactories * 2;
        if (pp.raw > pp.autoFactories * 4) {
            move.toProdAuto = pp.autoFactories * 4;
            int remaining = pp.raw - move.toProdAuto;
            move.toProdCommon = std::min(pp.commonFactories * 2, remaining);
        }
        else move.toProdAuto = std::min(pp.autoFactories * 4, pp.raw);

        // Подсчёт средних цен
        auto getAveragePrice = [](const QVector<request>& vec) -> int {
            if (vec.isEmpty()) return 10;
            int sum = 0;
            for (const request& r : vec) sum += r.cost;
            return sum / vec.size();
            };

        
        const infoTable& lastInfo = (it.isEmpty() ? infoTable() : it.last());

        int avgRawPrice = getAveragePrice(lastInfo.rawSold);
        int avgProdPrice = getAveragePrice(lastInfo.prodBought);

        // Покупка сырья
        if (currentTurn < 2) {
            move.rawBuyRequest = { b.rawSellBid.count, 10 };
        }
        else if (turnsLeft <= 2) {
            move.rawBuyRequest = { 0, 0 };
        }
        else {
            int maxProcessableRaw = (pp.commonFactories * 2 + pp.autoFactories * 4) * 2;

            if (pp.raw > maxProcessableRaw) {
                int bPrice = avgRawPrice - 4;
                move.rawBuyRequest = { b.rawSellBid.count, bPrice };
            
            }
            else {
                int rawBuyPrice = b.rawSellBid.cost + 4;
                move.rawBuyRequest = { maxProcessableRaw, rawBuyPrice };
            }
        }

        // Продажа ГП
        int totalProdSold = 0;
        for (const request& r : lastInfo.prodBought) totalProdSold += r.count;
        int prodPer2Turns = (lastInfo.prodBought.isEmpty() ? 0 : totalProdSold / lastInfo.prodBought.size());

        if (currentTurn < 3) {
            move.prodSellRequest = { pp.prod, 45 };
        }
        else if (turnsLeft <= 3) {
            move.prodSellRequest = { pp.prod, 30 };
        }
        else if (pp.prod > prodPer2Turns) {
            move.prodSellRequest = { pp.prod, 37 };
        }
        else if (b.prodBuyBid.cost > 0.9 * avgProdPrice) {
            move.prodSellRequest = { pp.prod, avgProdPrice };
        }
        else {
            move.prodSellRequest = { pp.prod, 37 };
        }
        //кредит 
        if (((pp.commonFactories + pp.autoFactories < 10) || pp.commonFactories > 0) && pp.balance < 400 && turnsLeft > 12) {
            move.loan = 1000 - pp.balance;
        }
        else move.loan = 0;
        return move;
    };
    

    QString getCode() {
        return "andrew";
    }
};
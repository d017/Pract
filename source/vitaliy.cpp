
#include <iostream>
#include "playeralgorithm.h"
#include "structures.h"
using namespace std;





class Vitaliy : public playerAlgorithm {
    QVector<pair<int, int>> bankBids; // 1 - rawSell 2 - prodBuy  вектор со ставками банка

public:

    Vitaliy() {};

    virtual playerMove getMove(playerProperty property, QVector<infoTable> table, bid bid) override {
        int month = table.size();     //месяц(ход)
        playerMove move;
        request buy;
        request sell;
        birthdayGift gift;

        buy.cost = 0; buy.count = 0; sell.cost = 0; sell.count = 0;             //все поля изначально  = 0, потом если нужно меняются
        move.toProdCommon = 0; move.toProdAuto = 0; move.buyFactories = 0; 
        move.factoriesToAuto = 0; move.loan = 0;
        move.prodSellRequest = sell; move.rawBuyRequest = buy;  
        gift.prod = 0; gift.money = 0; gift.raw = 0;                

        pair<int, int>  temp;
        temp.first = bid.rawSellBid.cost; temp.second = bid.prodBuyBid.cost;
        bankBids.append(temp);  // сохранение ставок банка 
        
        if (bid.happyCase.index == 4) {            // подарок
            if (property.prod > 0) { gift.prod = 1; }
            else { gift.money = 25; }
        }
        move.gift = gift;

        if (month < 3) {             //ходы до 3 месяца
            sell.cost = bid.prodBuyBid.cost - rand() % 10;
            sell.count = property.prod;
            return move;
        }
        if (property.prod < 1) {                // переработка ЕС
             move.toProdCommon = 1;
        }

        int sum = 0;

        if (property.prod < 1 && property.raw < 1) {        // покупка EC
            buy.count = 1;
            for (int i = 0; i < month; i++) {
                sum += bankBids[i].first;
            }
            buy.cost = (sum / month) + 1;
            move.rawBuyRequest = buy;
        }

        sum = 0;
        for (int i = 0; i < month; i++) {          // продажа ГП
            sum += bankBids[i].second;
        }
        sell.count = property.prod;
        sell.cost = (sum / month) + 1;
        move.prodSellRequest = sell;
                                                             
        return move;
    }
    virtual QString getCode() { return "polina"; };  // пароль

};


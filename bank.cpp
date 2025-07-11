#include "bank.h"

Bank::Bank(int numberOfPlayers, int gameMonths){
    currentFirst = 0;
    infoTable table;
    playerProperty temp ={{2,0,2000,2,2},{},{}};
    for(int i=0;i<numberOfPlayers;i++){
        properties.push_back(temp);
        table.playersTable.push_back(temp);
    }
    table.prodBought={};
    table.rawSold={};
    this->infoTables.push_back(table);
    this->gameLength=gameMonths;
}

int Bank::getPlayersCount() {
    return properties.size();
}

QVector<infoTable> Bank::getInfoTable(){
    return infoTables;
}

playerProperty Bank::getProperty(int index){
    return properties[index];
}

QPair<QVector<request>,QVector<request>> Bank::getMonthResults(QVector<playerMove> playerMoves){
    for(int i=0;i<properties.size();i++){
        properties[i].balance-=10*properties[i].prod+ 2*properties[i].raw;
        if(!infoTables.size()%12){
            properties[i].balance-=properties[i].autoFactories*100 + properties[i].commonFactories*50;
        }
        if(playerMoves[i].loan>0){
            moneyLoan loan={playerMoves[i].loan, 12};
            properties[i].moneyLoans.push_back(loan);
            properties[i].balance+=playerMoves[i].loan;
        }
    }
    if(currentBid.happyCase.index==4){
        for(int i=0;i<properties.size();i++){
            if(i!=currentBid.happyCase.target){
                properties[currentBid.happyCase.target].raw+=playerMoves[i].gift.raw;
                properties[currentBid.happyCase.target].prod+=playerMoves[i].gift.prod;
                properties[i].balance+=playerMoves[i].gift.prod * 10 + playerMoves[i].gift.raw * 2;
                properties[currentBid.happyCase.target].balance+=playerMoves[i].gift.money;
            }
        }
    }
    QVector<request> boughtProd;
    QVector<request> soldRaw;
    QVector<QPair<int,playerMove>> tmp;
    for(int i=0;i<playerMoves.size();i++){
        tmp.push_back({i,playerMoves[i]});
    }
    std::rotate(tmp.begin(),tmp.begin()+currentFirst,tmp.end());
    std::sort(tmp.begin(),tmp.end(),[](QPair<int,playerMove> a, QPair<int,playerMove> b){return a.second.prodSellRequest.cost<b.second.prodSellRequest.cost;});
    int sell=currentBid.prodBuyBid.count;
    int t=0;
    while(sell>0 && t<tmp.size()){
        int numb=std::min(sell,tmp[t].second.prodSellRequest.count);
        int fin;
        if(tmp[t].second.prodSellRequest.cost!=0)
            fin=std::min(numb,properties[tmp[t].first].balance/tmp[t].second.prodSellRequest.cost);
        else
            fin=0;
        sell-=fin;
        properties[tmp[t].first].balance+=fin*tmp[t].second.prodSellRequest.cost;
        properties[tmp[t].first].prod-=fin;
        if(fin>0)
            boughtProd.push_back({fin,tmp[t].second.prodSellRequest.cost});
        t++;
    }
    std::sort(tmp.begin(),tmp.end(),[](QPair<int,playerMove> a, QPair<int,playerMove> b){return a.second.rawBuyRequest.cost>b.second.rawBuyRequest.cost;});
    sell=currentBid.rawSellBid.count;
    t=0;
    while(sell>0 && t<tmp.size()){
        int numb=std::min(sell,tmp[t].second.rawBuyRequest.count);
        int fin;
        if(tmp[t].second.rawBuyRequest.cost!=0)
            fin=std::min(numb,properties[tmp[t].first].balance/tmp[t].second.rawBuyRequest.cost);
        else
            fin=0;
        sell-=fin;
        properties[tmp[t].first].balance-=fin*tmp[t].second.rawBuyRequest.cost;
        properties[tmp[t].first].raw+=fin; //wtf fixed
        if(fin>0)
            soldRaw.push_back({fin,tmp[t].second.rawBuyRequest.cost});
        t++;
    }
    for(int i=0;i<playerMoves.size();i++){
        int numb=std::min(properties[i].raw,playerMoves[i].toProdAuto);
        numb=std::min(numb,properties[i].autoFactories*4);
        int fin=std::min(numb,properties[i].balance/8);
        properties[i].balance-=fin*8;
        properties[i].raw-=fin;
        properties[i].prod+=fin;
        numb=std::min(properties[i].raw,playerMoves[i].toProdCommon);
        numb=std::min(numb,properties[i].commonFactories*2);
        fin=std::min(numb,properties[i].balance/20);
        properties[i].balance-=fin*20;
        properties[i].raw-=fin;
        properties[i].prod+=fin;
    }
    for(int i=0;i<playerMoves.size();i++){
        auto loansLeft = properties[i].moneyLoans;
        for(auto loan=properties[i].moneyLoans.begin();loan!=properties[i].moneyLoans.end();loan++){
            (*loan).monthsLeft--;
            if((*loan).monthsLeft<1){
                properties[i].balance-=(*loan).sum*1.1;
                //properties[i].moneyLoans.erase(loan);
            }
            else
                loansLeft.append(*loan);
        }
        properties[i].moneyLoans = loansLeft;
    }
    for(int i=0;i<playerMoves.size();i++){
        auto factoryLoansLeft = properties[i].factoryLoans;
        for(auto loan=properties[i].factoryLoans.begin();loan!=properties[i].factoryLoans.end();loan++){
            (*loan).monthsLeft--;
            if((*loan).monthsLeft<1){
                properties[i].autoFactories++;
                properties[i].commonFactories--;
                properties[i].balance-=(*loan).sum;
                //properties[i].factoryLoans.erase(loan);
            }
            else
                factoryLoansLeft.append(*loan);
        }
        properties[i].factoryLoans = factoryLoansLeft;
        if(playerMoves[i].factoriesToAuto>0){
            int numb = static_cast<int>(
                std::min<qsizetype>(
                    static_cast<qsizetype>(playerMoves[i].factoriesToAuto),
                    static_cast<qsizetype>(properties[i].commonFactories) - properties[i].factoryLoans.size()
                    )
                );
            numb=std::min(numb,properties[i].balance/300);
            for(int j=0;j<numb;j++){
                properties[i].factoryLoans.push_back({300,9});
                properties[i].balance-=300;
            }
        }
    }
    for(int i=0;i<playerMoves.size();i++){
        int numb=std::min(5-properties[i].autoFactories-properties[i].commonFactories,playerMoves[i].buyFactories);
        numb=std::min(numb,properties[i].balance/100);
        properties[i].balance-=100*numb;
        properties[i].commonFactories+=numb;
    }
    for(int i=0;i<playerMoves.size();i++){
        if(properties[i].balance<0){
            properties[i]={{0,0,properties[i].balance,0,0},{},{}};
        }
    }
    infoTable table;
    table.prodBought=boughtProd;
    table.rawSold=soldRaw;

    for(int i=0;i<properties.size();i++){
        table.playersTable.push_back(properties[i]);
    }
    infoTables.push_back(table);
    return {soldRaw,boughtProd};
}

bool Bank::isAlive(int playerIndex){
    return properties[playerIndex].balance>=0;
}

bool Bank::isEndGame(){
    int count=0;
    for(int i=0;i<properties.size();i++){
        if(properties[i].balance>=0)
            count++;
    }
    if(count<2)
        return true;
    if(infoTables.size()>=gameLength)
        return true;
    return false;
}

QVector<int> Bank::calculatePoints(){
    QVector<int> rez;
    for(int i=0;i<properties.size();i++){
        int score=properties[i].balance-properties[i].factoryLoans.size()*300+properties[i].raw*5+properties[i].prod*30+properties[i].autoFactories*250+properties[i].commonFactories*50;
        for(auto loan=properties[i].moneyLoans.begin();loan!=properties[i].moneyLoans.end();loan++){
            score-=(*loan).sum*1.1;
        }
        rez.push_back(score);
    }
    return rez;
}

int Bank::currState(int randomNum, const int weights[5]) {
    int percent = 0;
    for (int i = 0; i < 5; i++) {
        percent += weights[i];
        if (randomNum <= percent){
            return i + 1;
        }
    }
    return 5;
}

bid Bank::makeBids() {
    int state;
    const int kpercentProd[5] = {10, 30, 50, 70, 90};
    const int kpercentRaw[5] = {90, 70, 50, 30, 10};

    const int percentRaw[5] = {80, 65, 50, 35, 20};
    const int percentProd[5] = {20, 35, 50, 65, 80};
    if (infoTables.size() == 1) {
        state = 3;
    }
    else {
        const auto& last = infoTables.back().playersTable;
        int totalRaw = 0, totalProd = 0;
        for (auto& p: last) {
            totalProd += p.prod;
            totalRaw += p.raw;
        }
        double solve;
        //проверка на 0
        if (totalRaw != 0) {
            solve = double(totalProd) / totalRaw;
        }
        else {
            solve = 999.0;
        }

        if (solve >= 3) {
            state = 5;
        }
        else if (solve >= 2) {
            state = 4;
        }
        else if (solve >= 1) {
            state = 3;
        }
        else if (solve >= 0.5) {
            state = 2;
        }
        else {
            state = 1;
        }
    }
    int randomNum = std::rand() % 100 + 1;
    state = currState(randomNum, transition[state - 1]);
    const auto& last = infoTables.back().playersTable;
    int totalRaw = 0, totalProd = 0;
    for (auto& p : last) {
        totalRaw +=p.raw;
        totalProd += p.prod;
    }
    if (totalRaw == 0) {
        totalRaw = last.size() * 3;
    }
    int k1 = kpercentRaw[state - 1];
    int k2 = kpercentProd[state - 1];

    int fRaw = totalRaw * k1 * 15 / 100;
    int fProd = totalProd * k2 * 90 / 100;

    // int wantRaw = totalRaw * percentRaw[state - 1] /100;
    // int wantProd = totalProd * percentProd[state - 1] / 100;
    int wantRaw = totalRaw * percentRaw[state - 1] /100 + 1;
    int wantProd = totalProd * percentProd[state - 1] / 100 + 1;

    int priceRaw = fRaw / wantRaw;
    int priceProd = wantProd > 0 ? fProd / wantProd : 0;

    currentBid.rawSellBid = {wantRaw, priceRaw};
    currentBid.prodBuyBid = {wantProd, priceProd};

    randomNum = rand() % 100 + 1;
    if (randomNum < 16) {
        randomNum = rand() % 4 + 1;
        if (randomNum == 1) {
            randomNum = rand() % properties.size();
            while (properties[randomNum].balance < 0 && properties[randomNum].commonFactories < 1 && properties[randomNum].autoFactories < 1) {
                randomNum = rand() % properties.size();
            }
            if (properties[randomNum].commonFactories > 0)
                properties[randomNum].commonFactories--;
            else
                properties[randomNum].autoFactories--;
            currentBid.happyCase = { 1,randomNum };
        }
        else if (randomNum == 2) {
            randomNum = rand() % properties.size();
            while (properties[randomNum].balance < 0 && properties[randomNum].commonFactories + properties[randomNum].autoFactories < 5) {
                randomNum = rand() % properties.size();
            }
            properties[randomNum].commonFactories++;
            currentBid.happyCase = { 2,randomNum };
        }
        else if (randomNum == 3) {
            randomNum = rand() % properties.size();
            while (properties[randomNum].balance < 0) {
                randomNum = rand() % properties.size();
            }
            properties[randomNum].balance += 200;
            currentBid.happyCase = { 3,randomNum };
        }
        else if (randomNum == 4) {
            randomNum = rand() % properties.size();
            while (properties[randomNum].balance < 0) {
                randomNum = rand() % properties.size();
            }
            currentBid.happyCase = { 4,randomNum };
        }
    }
    else
        currentBid.happyCase = {0, 0};

    return currentBid;
}

#ifndef BANK_H
#define BANK_H

#include "structures.h"
#include <QVector>

class Bank{
private:
    QVector<playerProperty> properties;
    QVector<infoTable> infoTables;
    bid currentBid;
    int currentFirst;
    int gameLength;
    static constexpr int transition[5][5] = {
        {40, 33, 15, 8, 4},
        {22, 40, 22, 10, 6},
        {10, 20, 40, 20, 10},
        {6, 10, 22, 40, 22},
        {4, 8, 15, 33, 40}
    };
public:
    Bank(int numberOfPlayers, int gameLength);

    int getPlayersCount();

    QVector<infoTable> getInfoTable();

    playerProperty getProperty(int index);

    QPair<QVector<request>,QVector<request>> getMonthResults(QVector<playerMove> playerMoves);

    bool isAlive(int playerIndex);

    bool isEndGame();

    QVector<int> calculatePoints();

    bid makeBids();

    int currState(int randomNum, const int weights[5]);
};


#endif // BANK_H

#ifndef POLINA_H
#define POLINA_H

#include "playerAlgorithm.h"

class Polina : public playerAlgorithm {
private:
    int currentMonth = 1;

public:

    void nextMonth() {
        this->currentMonth++;
    }
    Polina();
    int calculateSafeBalance(const playerProperty&);
    playerMove getMove(playerProperty properties, QVector<infoTable> info, bid currentBid) override;
    QString getCode() override;
};

#endif //POLINA_H

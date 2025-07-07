#ifndef PLAYERALGORITHM_H
#define PLAYERALGORITHM_H

#include <structures.h>
#include <QString>

class playerAlgorithm
{
public:
    playerAlgorithm() {};

    virtual playerMove getMove(playerProperty, QVector<infoTable>, bid) = 0;

    virtual QString getCode() = 0;
};

#endif // PLAYERALGORITHM_H

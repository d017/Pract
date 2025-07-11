#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#pragma once

#include <QVector>
#include "time.h"
#include "structures.h"

#include "prokhor.cpp"        //бібліятэкі з алгарытмамі гульцоў
#include "andrew.cpp"         //(назвы алгарытмаў)
#include "vitaliy.cpp"
#include "polina.h"

struct playerEntity         //структура "рэгістрацыі" гульца
{
    playerEntity() {
        algorithm = nullptr;
        type = human;
    }
    enum playerType {comp, human} type;
    playerAlgorithm* algorithm;
};

class playerManager //: public Prokhor, public Andrew, public Vitaliy, public Polina   //падлючаем класы алгарытмаў
{
    //Q_OBJECT
private:
    QVector<playerEntity> players;

public:
    playerManager();                      //функцыя "рэгістрацыі" гульца
    void addPlayer(QString code);
    void addBot();
    void addPlayerWithoutAlgorithm();
    std::optional<playerMove> getAlgorithmMove(int PlayerInteger, playerProperty playerProp, QVector<infoTable> vecInfoTable, bid genBid); //выклік хода
    bool isHuman(int);
    ~playerManager();
};
#endif // PLAYERMANAGER_H

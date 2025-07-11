#include "playermanager.h"

playerManager::playerManager()
{
    players = QVector<playerEntity>(0);
}

void playerManager::addPlayer(QString code)
{
    playerEntity newPlayerEnt;   //ствараем пустога гульца
    newPlayerEnt.type = playerEntity::human;                      //прысвойваем тып чалавек
    if (code == "andrew")                               //па паролі шукаем алгарытм
        newPlayerEnt.algorithm = new Prokhor;
    else if (code == "prokhor")
        newPlayerEnt.algorithm = new Andrew;
    else if (code == "polina")
        newPlayerEnt.algorithm = new Vitaliy;
    else if (code == "vitaliy")
        newPlayerEnt.algorithm = new Polina;
    else
        newPlayerEnt.algorithm = nullptr;
    players.push_back(newPlayerEnt);                //дадаём у вектар гульца
}

void playerManager::addBot()
{
    playerEntity newPlayerEnt;   //ствараем пустога гульца
    newPlayerEnt.type = playerEntity::comp;                       //прысвойваем тып бот
    srand(time(NULL));                              //рандомам абіраем алгарытм
    int algBot = rand()%4+1;
    switch (algBot) {
    case 1:
        newPlayerEnt.algorithm = new Prokhor;
        break;
    case 2:
        newPlayerEnt.algorithm = new Andrew;
        break;
    case 3:
        newPlayerEnt.algorithm = new Vitaliy;
        break;
    case 4:
        newPlayerEnt.algorithm = new Polina;
        break;
    default:
        break;
    }
    players.push_back(newPlayerEnt);                //дадаём у вектар гульца
}

void playerManager:: addPlayerWithoutAlgorithm()
{
    playerEntity newPlayerEnt;   //ствараем пустога гульца
    newPlayerEnt.type = playerEntity::human;                      //прысвойваем тып чалавек
    newPlayerEnt.algorithm = nullptr;
    players.push_back(newPlayerEnt);                //дадаём у вектар гульца
}

QString playerManager::getPlayerCode(int index) {
    return players[index].algorithm->getCode();
}

bool playerManager::hasAlgorithm(int index) {
    if (players[index].algorithm == nullptr)
        return false;
    return true;
}

std::optional<playerMove> playerManager::getAlgorithmMove(int playerInteger, playerProperty playerProp, QVector<infoTable> vecInfoTable, bid genBid)
{
    playerAlgorithm* currPlayerAlg = players[playerInteger].algorithm;   //знаходзім алгарытм гульца пад патрэбным нумарам
    if (dynamic_cast<Prokhor*>(currPlayerAlg)!=nullptr)
        currPlayerAlg=dynamic_cast<Prokhor*>(currPlayerAlg);
    else if (dynamic_cast<Andrew*>(currPlayerAlg)!=nullptr)
            currPlayerAlg=dynamic_cast<Andrew*>(currPlayerAlg);
    else if(dynamic_cast<Vitaliy*>(currPlayerAlg)!=nullptr)
            currPlayerAlg=dynamic_cast<Vitaliy*>(currPlayerAlg);
    else if(dynamic_cast<Polina*>(currPlayerAlg)!=nullptr)
            currPlayerAlg=dynamic_cast<Polina*>(currPlayerAlg);
    else
        currPlayerAlg = nullptr;
    if (currPlayerAlg == nullptr)
        return std::nullopt;
    playerMove newPlayerMove = currPlayerAlg->getMove(playerProp, vecInfoTable, genBid);     //выклікаем ход
    return newPlayerMove;
}

bool playerManager::isHuman(int playerIndex) {
    if (players[playerIndex].type == playerEntity::human)
        return true;
    return false;
}

playerManager::~playerManager() {}

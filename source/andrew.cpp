#pragma once
#include <iostream>
#include <cmath>
#include "structures.h"
#include "playeralgorithm.h"


using namespace std;

class Andrew : public playerAlgorithm
{

    int credit(playerProperty& pp,QVector<infoTable> &it)
    {
        int cred;
        int m_average=0;
        int sum=0;
        for (int i = it.size(); i > it.size()-3 && i >= 0; i--)
        {
            for (int j = 0; j < it[i-1].prodBought.size(); j++)
            {
                sum += it[i - 1].prodBought[j].cost;
            }
            m_average += sum / it[i - 1].prodBought.size();
            sum = 0;
        }
        m_average = m_average / 3;
        cred = ((pp.autoFactories * 4 + 2 * (pp.commonFactories + 1)) * m_average - (20 * pp.commonFactories + 8 * pp.autoFactories))*9/10;
        return cred;
    };

    void production(playerProperty &pp, playerMove &move)
    {
        int rawLeft = pp.raw;
        if ((pp.autoFactories > 0 || pp.commonFactories > 0) && rawLeft > 0)
        {
            if(pp.autoFactories > 0)
            {
                if(pp.autoFactories*4>=pp.raw)
                {
                    move.toProdAuto += pp.raw;
                }
                else
                {
                    move.toProdAuto += 4 * pp.autoFactories;
                }
                rawLeft -= move.toProdAuto;
            }
            if (pp.commonFactories > 0 && rawLeft > 0)
            {
                if (pp.commonFactories * 2 >= rawLeft)
                {
                    move.toProdCommon += rawLeft;
                }
                else
                {
                    move.toProdCommon += 2 * pp.autoFactories;
                }
            }
        }
    };

    void buyraw(playerProperty& pp, QVector<infoTable>& it, playerMove& move)
    {
        int m_average = 0;
        int sum = 0;
        if(it.size()<3)
        {
            return;
        }
        for (int i = it.size(); i > it.size() - 3; i--)
        {
            for (int j = 0; j < it[i - 1].rawSold.size(); j++)
            {
                sum += it[i - 1].rawSold[j].cost;
            }
            if (it[i - 1].rawSold.size() != 0)
                m_average += sum / it[i - 1].rawSold.size();
            sum = 0;
        }
        m_average = m_average / 3;
        int rowcount = ((4 * pp.autoFactories + 2 * pp.commonFactories) - pp.raw)*12/10;
        int rowcost;
        if (it[it.size()-1].rawSold.size() == 0 || it[it.size()-2].rawSold.size() == 0 || it[it.size()-3].rawSold.size() == 0)
            rowcost = m_average;
        else if(it[it.size()-1].rawSold[0].cost> it[it.size() - 2].rawSold[0].cost && it[it.size() - 2].rawSold[0].cost< it[it.size() - 3].rawSold[0].cost)
        {
            rowcost = m_average * 9 / 10;
        }
        else
        {
            rowcost = m_average * 115 / 100;
        }
        move.rawBuyRequest = { rowcount,rowcost };
    };

    void forsell(playerProperty& pp, QVector<infoTable>& it, playerMove& move)
    {
        int x, y, z;
        int average=0;
        if(it.size()>=3)
        {
            for (int i = it.size(); i > it.size() - 3; i--)
            {
                for (int j = 0; j < it[i - 1].prodBought.size(); j++)
                {
                    average += it[i - 1].prodBought[j].cost;
                }
                if (it[i - 1].prodBought.size() != 0)
                    average = average / it[i - 1].prodBought.size();
                if (i == it.size())
                {
                    x = average;
                    average = 0;
                }
                else if (i == it.size() - 1)
                {
                    y = average;
                    average = 0;
                }
                else if (i == it.size() - 2)
                {
                    z = average;
                    average = 0;
                }
            }
            if (x > z && y > z)
            {
                move.prodSellRequest = { pp.prod,((x + y + z) / 3) * 115 / 100 };
            }
            else if (fabs((x + y + z) / 3 - (y + z + average) / 3) < 5)
            {
                move.prodSellRequest = { pp.prod / 2, (x + y + z) / 3 };
            }
            else if (x < z && y < z)
            {
                if (pp.prod > 4)
                {
                    move.prodSellRequest = { pp.prod / 4, ((x + y + z) / 3) * 85 / 100 };
                }
                else
                {
                    move.prodSellRequest = { 1, ((x + y + z) / 3) * 85 / 100 };
                }

            }
        }
    };


    playerMove getMove(playerProperty pp, QVector<infoTable> it, bid deposit)
    {
        playerMove move;
        move.buyFactories = 0;
        move.factoriesToAuto = 0;
        move.gift = { 0,0,0 };
        move.loan = 0;
        move.prodSellRequest = { 0,0 };
        move.rawBuyRequest = { 0,0 };
        move.toProdAuto = 0;
        move.toProdCommon = 0;
        ///////////////Первая проверка(проверка дня рождения)/////////////////
        if (deposit.happyCase.index == 4)
        {
            if (pp.prod > 1 && it.size() > 5)
            {
                move.gift.prod= 1;
            }
            else
            {
                move.gift.money= 30;
            }
        }

        ///////////////Вторая проверка(кол-во фабрик > 1)/////////////////
        if (pp.commonFactories > 1)
        {
            if (it.size()%6==0 && (pp.commonFactories+pp.autoFactories)<5)
            {
                if (pp.balance < 500)
                {
                    move.loan = credit(pp, it);
                }
                move.buyFactories = 1;
                if (pp.commonFactories > 3)
                {
                    move.factoriesToAuto = 1;
                }
            }
        }
        else
        {
            if (pp.balance < 500)
            {
                move.loan = credit(pp, it);
            }
            move.buyFactories = 1;
            if (pp.commonFactories > 3)
            {
                move.factoriesToAuto = 1;
            }
        }
        ///////////////Третяя проверка(кол-во ЕС>0)/////////////////
        if (pp.raw>0)
        {
            production(pp,move);
        }
        ///////////////Четвертая проверка(кол-во ЕС>0)/////////////////
        if(pp.prod>0)
        {
            forsell(pp,it,move);
            if(pp.raw<(4*pp.autoFactories+2*pp.commonFactories))
            {
                buyraw(pp,it,move);
            }
        }
        else
        {
            buyraw(pp,it,move);
        }
        return move;
    };

    QString getCode() { return "prokhor";};
};

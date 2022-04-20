#include "GameSignals.h"

GameSignals *GameSignals::instance = 0;

GameSignals::GameSignals(QObject *parent)
    : QObject{parent}
{

}

GameSignals *GameSignals::getInstance()
{
    if (instance == 0) {
        instance = new GameSignals();
    }
    return instance;
}

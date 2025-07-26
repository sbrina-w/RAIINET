#include "gamemodel.h"

GameModel::GameModel()
    : board(),
      currentTurn(1)
{
    // create two players with IDs 1 and 2
    players.push_back(new Player(1));
    players.push_back(new Player(2));
}

GameModel::~GameModel()
{
    for (Player *p : players)
        delete p;
}

bool GameModel::isGameOver()
{
    // stub for now
    return false;
}

void GameModel::moveLink(char /*id*/, int /*dir*/)
{
    // TODO: implement movement
    // …
    notifyObservers(ChangeEvent::LinkMoved);
}

void GameModel::useAbility(int /*playerID*/, int /*abilityID*/, int /*target*/)
{
    // TODO: implement abilities
    // …
    notifyObservers(ChangeEvent::AbilityUsed);
}

// Observer management
void GameModel::addObserver(Observer *o)
{
    attach(o);
}
void GameModel::removeObserver(Observer *o)
{
    detach(o);
}

// Notify wrapper
void GameModel::notify(ChangeEvent e)
{
    notifyObservers(e);
}

// Getters for views
Board &GameModel::getBoard()
{
    return board;
}
const Board &GameModel::getBoard() const
{
    return board;
}
int GameModel::getCurrentTurn() const
{
    return currentTurn;
}

Player* GameModel::getCurrentPlayer() {
    return players[(currentTurn - 1) % players.size()];
}

#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "board.h"
#include "player.h"
#include "subject.h"
#include "changeevent.h"

class GameModel : public Subject {
private:
    Board board;
    std::vector<Player*> players;
    int currentTurn;

public:
    GameModel();
    ~GameModel();
    void moveLink(char id, int dir);
    void useAbility(int playerID, int abilityID, int target);
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notify(ChangeEvent event);
    bool isGameOver();
};

#endif

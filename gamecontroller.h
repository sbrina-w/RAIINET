#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "gamemodel.h"
#include "changeevent.h"
#include <string>

class GameController {
private:
    GameModel& model;
    
    //command handling methods
    void handleMove(char linkId, const std::string& direction);
    void handleAbilities();
    void handleAbility(int abilityId);
    void handleBoard();
    void handleSequence(const std::string& filename);

public:
    GameController(GameModel& model);
    void play();
    void notify(ChangeEvent event);
};

#endif

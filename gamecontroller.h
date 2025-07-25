#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "gamemodel.h"
#include "changeevent.h"

class GameController {
private:
    GameModel& model;

public:
    GameController(GameModel& model);
    void play();
    void notify(ChangeEvent event);
};

#endif

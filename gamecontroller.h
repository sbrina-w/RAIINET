#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "gamemodel.h"
#include "changeevent.h"
#include <string>

class GameController {
private:
    GameModel& model;
    std::vector<std::string> commandHistory;

    // helper functions
    static std::string abilityFullName(char id);

    // command history
    void writeCommandHistory();
    
    //command handling methods
    bool handleMove(char linkId, const std::string& direction, const std::string& originalLine);
    void handleAbilities();
    bool handleAbility(int abilityId, const std::vector<std::string>& args, const std::string& originalLine);
    void handleBoard();
    void handleSequence(const std::string& filename);

public:
    GameController(GameModel& model);
    void play();
    void notify(ChangeEvent event);
};

#endif

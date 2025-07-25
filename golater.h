#ifndef GOLATER_H
#define GOLATER_H

#include "ability.h"

class GoLater : public Ability {
public:
    GoLater();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

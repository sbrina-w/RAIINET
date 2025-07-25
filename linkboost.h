#ifndef LINKBOOST_H
#define LINKBOOST_H

#include "ability.h"

class LinkBoost : public Ability {
public:
    LinkBoost();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

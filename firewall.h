#ifndef FIREWALL_H
#define FIREWALL_H

#include "ability.h"

class Firewall : public Ability {
public:
    Firewall();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

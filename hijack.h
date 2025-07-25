#ifndef HIJACK_H
#define HIJACK_H

#include "ability.h"

class Hijack : public Ability {
public:
    Hijack();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

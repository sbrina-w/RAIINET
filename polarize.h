#ifndef POLARIZE_H
#define POLARIZE_H

#include "ability.h"

class Polarize : public Ability {
public:
    Polarize();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

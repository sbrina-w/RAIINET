#ifndef SCAN_H
#define SCAN_H

#include "ability.h"

class Scan : public Ability {
public:
    Scan();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

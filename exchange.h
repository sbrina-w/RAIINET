#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "ability.h"

class Exchange : public Ability {
public:
    Exchange();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

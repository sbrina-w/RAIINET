#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "ability.h"

class Download : public Ability {
public:
    Download();
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

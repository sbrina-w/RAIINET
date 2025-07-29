#ifndef EXCHANGE_H
#define EXCHANGE_H

#include "ability.h"
#include "gamemodel.h"
#include "link.h"
#include "board.h"

class Exchange : public Ability {
public:
    Exchange();
    // helper for triggering potential firewall effect after exchange
    static bool checkFirewallAfterPlacement(GameModel& model, Board& board, int r, int c, Link* link);
    void execute(GameModel& model, std::vector<std::string> args) override;
};

#endif

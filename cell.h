#ifndef CELL_H
#define CELL_H

#include "celltype.h"

class Link;
class Player;

class Cell {
private:
    Link* link;
    Player* firewallOwner;
    CellType cellType;

public:
    Cell();
    CellType getCellType();
    void setCellType(CellType type);
    Player* getFirewallOwner();
    void setFirewallOwner(Player* owner);
    Link* getLink();
    void setLink(Link* link);
};

#endif

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
    CellType getCellType() const;
    void setCellType(CellType type);
    Player* getFirewallOwner() const;
    void setFirewallOwner(Player* owner);
    Link* getLink() const;
    void setLink(Link* link);
    void removeLink();
};

#endif

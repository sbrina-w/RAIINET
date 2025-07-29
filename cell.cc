#include "cell.h"

Cell::Cell()
    : link(nullptr),
      firewallOwner(nullptr),
      cellType(CellType::Normal)
{
}

CellType Cell::getCellType() const
{
    return cellType;
}

void Cell::setCellType(CellType type)
{
    cellType = type;
}

Player *Cell::getFirewallOwner() const
{
    return firewallOwner;
}

void Cell::setFirewallOwner(Player *owner)
{
    firewallOwner = owner;
}

Link *Cell::getLink() const
{
    return link;
}

void Cell::setLink(Link *lnk)
{
    link = lnk;
}

void Cell::removeLink()
{
    link = nullptr;
}

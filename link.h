#ifndef LINK_H
#define LINK_H

#include "linktype.h"

class Player;

class Link
{
private:
    char id; // a-h or A - H
    LinkType linkType;
    bool boosted;
    int strength;
    bool revealed;
    Player *owner;

public:
    Link(char id, LinkType type, int strength, Player *owner);

    char getId() const;
    int getStrength() const;
    LinkType getType() const;
    bool isBoosted() const;
    bool isRevealed() const;
    Player *getOwner() const;

    void reveal();
    void switchType();
    void setBoosted(bool b);
};

#endif

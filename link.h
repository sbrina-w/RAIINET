#ifndef LINK_H
#define LINK_H

#include "linktype.h"

class Link {
private:
    LinkType linkType;
    bool isBoosted;
    int strength;

public:
    Link();
    int getStrength();
    LinkType getType();
    void reveal();
    void switchType();
    bool getBoostedStatus();
};

#endif

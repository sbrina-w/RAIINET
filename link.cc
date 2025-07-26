#include "link.h"
#include "player.h"

Link::Link(char id, LinkType type, int strength, Player* owner)
  : id(id)
  , linkType(type)
  , boosted(false)
  , strength(strength)
  , revealed(false)
  , owner(owner)
{}

char Link::getId() const {
    return id;
}

int Link::getStrength() const {
    return strength;
}

LinkType Link::getType() const {
    return linkType;
}

bool Link::isBoosted() const {
    return boosted;
}

bool Link::isRevealed() const {
    return revealed;
}

Player* Link::getOwner() const {
    return owner;
}

void Link::reveal() {
    revealed = true;
}

void Link::switchType() {
    linkType = (linkType == LinkType::Data ? LinkType::Virus : LinkType::Data);
}

void Link::setBoosted(bool b) {
    boosted = b;
}
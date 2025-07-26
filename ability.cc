#include "ability.h"

Ability::Ability(char id) : used(false), abilityID(id) {}

bool Ability::isUsed() {
    return used;
}

char Ability::getID() {
    return abilityID;
}

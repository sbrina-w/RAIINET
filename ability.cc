#include "ability.h"

Ability::Ability(char id) : used(false), abilityID(id) {}

bool Ability::isUsed() const {
    return used;
}

char Ability::getID() const {
    return abilityID;
}

void Ability::markUsed() {
    used = true;
}

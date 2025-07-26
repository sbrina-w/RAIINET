#include "player.h"

Player::Player(int playerId)
  : id(playerId),
    downloadedData(0),
    downloadedVirus(0),
    allowTwoAbilities(false)
{}

int Player::getId() const {
    return id;
}

int Player::getDataDownloadCount() const {
    return downloadedData;
}

int Player::getVirusDownloadCount() const {
    return downloadedVirus;
}

void Player::playAbility(int abilityInd, int target) {
    // stub for now
}

void Player::chooseAbility(const std::string& abilities) {
    // stub for now
}

#include "player.h"
#include "link.h"

Player::Player(int playerId)
  : id(playerId),
    abilitiesRemaining(5),
    downloadedData(0),
    downloadedVirus(0),
    allowTwoAbilities(false)
{
    //initialize with default abilities
    setAbilities("LFDSP");
}

Player::~Player() {
    //clean up owned links
    for (auto& pair : links) {
        delete pair.second;
    }
}

int Player::getId() const {
    return id;
}

int Player::getDataDownloadCount() const {
    return downloadedData;
}

int Player::getVirusDownloadCount() const {
    return downloadedVirus;
}

int Player::getUnusedAbilityCount() const {
    return abilitiesRemaining;
}

void Player::addLink(char linkId, Link* link) {
    links[linkId] = link;
}

Link* Player::getLink(char linkId) const {
    auto it = links.find(linkId);
    return (it != links.end()) ? it->second : nullptr;
}

const std::map<char, Link*>& Player::getLinks() const {
    return links;
}

void Player::learnOpponentLink(char linkId, Link* link) {
    opponentLinks[linkId] = link;
}

Link* Player::getKnownOpponentLink(char linkId) const {
    auto it = opponentLinks.find(linkId);
    return (it != opponentLinks.end()) ? it->second : nullptr;
}

bool Player::knowsOpponentLink(char linkId) const {
    return opponentLinks.find(linkId) != opponentLinks.end();
}

//fix this later, change to vector of abilities?
void Player::setAbilities(const std::string& abilityOrder) {
    abilities.clear();
    for(char c : abilityOrder) {
        abilities.push_back(c);
    }
}

void Player::playAbility(int abilityInd, int target) {
    // stub for now
}

//could remove?
void Player::chooseAbility(const std::string& abilities) {
    // stub for now
}

#include "player.h"
#include "link.h"
// include abilities
#include "linkboost.h"
#include "firewall.h"
#include "download.h"
#include "scan.h"
#include "polarize.h"
#include "exchange.h"
#include "golater.h"
#include "hijack.h"

Player::Player(int playerId)
  : id(playerId),
    abilitiesRemaining(5),
    downloadedData(0),
    downloadedVirus(0),
    goLaterActive(false)    
{
    //initialize with default abilities
    setAbilities("LFDSP");
}

Player::~Player() {
    // clean up owned links
    for (auto& pair : links) {
        delete pair.second;
    }
    // clean up abilities
    for (Ability* a : abilities) delete a;
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

void Player::incrementDownload(LinkType t){
    if (t == LinkType::Data){
        downloadedData += 1;
    } else {
        downloadedVirus += 1;
    }
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

void Player::setAbilities(const std::string& abilityOrder) {
    // Clean up old abilities if needed
    for (Ability* a : abilities) delete a;
    abilities.clear();

    for (char c : abilityOrder) {
        Ability* ability = nullptr;
        switch (c) {
            case 'L': ability = new LinkBoost(); break;
            case 'F': ability = new Firewall(); break;
            case 'D': ability = new Download(); break;
            case 'S': ability = new Scan(); break;
            case 'P': ability = new Polarize(); break;
            case 'E': ability = new Exchange(); break;
            case 'G': ability = new GoLater(); break;
            case 'H': ability = new Hijack(); break;
            default: break;
        }
        if (ability) abilities.push_back(ability);
    }
}

void Player::incrementAbilitiesUsed() {
    ++abilitiesUsedThisTurn;
}

void Player::decrementAbilitiesRemaining() {
    if (abilitiesRemaining > 0) {
        --abilitiesRemaining;
    }
}

bool Player::canUseAbility() const {
    return abilitiesUsedThisTurn < abilitiesAllowedThisTurn;
}

void Player::activateGoLater() {
    goLaterActive = true;
}

const std::vector<Ability*>& Player::getAbilities() const {
    return abilities;
}

void Player::startTurn() {
    abilitiesUsedThisTurn = 0;
    abilitiesAllowedThisTurn = goLaterActive ? 2 : 1;
    goLaterActive = false;
}

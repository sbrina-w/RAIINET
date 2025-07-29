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
#include <memory>

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

Player::~Player(){
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

void Player::addLink(char linkId, std::unique_ptr<Link> link) {
    links[linkId] = std::move(link);
}

Link* Player::getLink(char linkId) const {
    auto it = links.find(linkId);
    return (it != links.end()) ? it->second.get() : nullptr;
}

void Player::incrementDownload(LinkType t){
    if (t == LinkType::Data){
        downloadedData += 1;
    } else {
        downloadedVirus += 1;
    }
}

const std::map<char, std::unique_ptr<Link>>& Player::getLinks() const {
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
    abilities.clear();

    for (char c : abilityOrder) {
        std::unique_ptr<Ability> ability;
        switch (c) {
            case 'L': ability = std::make_unique<LinkBoost>(); break;
            case 'F': ability = std::make_unique<Firewall>(); break;
            case 'D': ability = std::make_unique<Download>(); break;
            case 'S': ability = std::make_unique<Scan>(); break;
            case 'P': ability = std::make_unique<Polarize>(); break;
            case 'E': ability = std::make_unique<Exchange>(); break;
            case 'G': ability = std::make_unique<GoLater>(); break;
            case 'H': ability = std::make_unique<Hijack>(); break;
            default: break;
        }
        if (ability) abilities.push_back(std::move(ability));
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

const std::vector<std::unique_ptr<Ability>>& Player::getAbilities() const {
    return abilities;
}

void Player::startTurn() {
    abilitiesUsedThisTurn = 0;
    abilitiesAllowedThisTurn = goLaterActive ? 2 : 1;
    goLaterActive = false;
}

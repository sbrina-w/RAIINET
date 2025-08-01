#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <vector>
#include <string>
#include "ability.h"
#include "linktype.h"
#include <memory>

class Link;
class Ability;

class Player
{
private:
    int id;
    std::map<char, std::unique_ptr<Link>> links;
    std::map<char, Link *> opponentLinks;
    std::vector<std::unique_ptr<Ability>> abilities;
    int abilitiesRemaining;
    int downloadedData;
    int downloadedVirus;

    // ability usage tracking
    bool goLaterActive = false;
    int abilitiesUsedThisTurn = 0;
    int abilitiesAllowedThisTurn = 1;

public:
    Player(int playerId);
    ~Player();
    
    // getters
    int getId() const;
    int getDataDownloadCount() const;
    int getVirusDownloadCount() const;
    int getUnusedAbilityCount() const;

    // for downloading:
    void incrementDownload(LinkType t);

    // link management
    void addLink(char linkId, std::unique_ptr<Link> link);
    Link* getLink(char linkId) const;
    const std::map<char, std::unique_ptr<Link>>& getLinks() const;

    // opponent link knowledge
    void learnOpponentLink(char linkId, Link* link);
    Link* getKnownOpponentLink(char linkId) const;
    bool knowsOpponentLink(char linkId) const;

    // ability management
    void setAbilities(const std::string& abilityOrder);
    void incrementAbilitiesUsed();
    void decrementAbilitiesRemaining();
    bool canUseAbility() const;
    void activateGoLater();
    const std::vector<std::unique_ptr<Ability>>& getAbilities() const;
    void startTurn();
};

#endif

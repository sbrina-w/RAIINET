#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <vector>
#include <string>
#include "ability.h"
#include "linktype.h"

class Link;
class Ability;

class Player
{
private:
    int id;
    std::map<char, Link *> links;
    std::map<char, Link *> opponentLinks;
    std::vector<Ability*> abilities;
    int abilitiesRemaining;
    int downloadedData;
    int downloadedVirus;
    bool allowTwoAbilities;

public:
    Player(int playerId);
    ~Player();
    
    //getters
    int getId() const;
    int getDataDownloadCount() const;
    int getVirusDownloadCount() const;
    int getUnusedAbilityCount() const;

    //for downloading:
    void incrementDownload(LinkType t);

    //link management
    void addLink(char linkId, Link* link);
    Link* getLink(char linkId) const;
    const std::map<char, Link*>& getLinks() const;

    //opponent link knowledge
    void learnOpponentLink(char linkId, Link* link);
    Link* getKnownOpponentLink(char linkId) const;
    bool knowsOpponentLink(char linkId) const;

    //ability management
    void setAbilities(const std::string& abilityOrder);
    
    void playAbility(int abilityInd, int target);
    void chooseAbility(const std::string& abilities);

    // getter for abilities, use for debugging
    const std::vector<Ability*>& getAbilities() const { return abilities; }
};

#endif

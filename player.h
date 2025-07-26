#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <vector>
#include <string>

class Link;
class Ability;

class Player
{
private:
    int id;
    std::map<char, Link *> links;
    std::map<char, Link *> opponentLinks;
    std::vector<char> abilities;
    int downloadedData;
    int downloadedVirus;
    bool allowTwoAbilities;

public:
    Player(int playerId);
    int getId() const;
    int getDataDownloadCount() const;
    int getVirusDownloadCount() const;
    void playAbility(int abilityInd, int target);
    void chooseAbility(const std::string& abilities);
};

#endif

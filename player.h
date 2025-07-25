#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <vector>
#include <string>

class Link;
class Ability;

class Player {
private:
    std::map<char, Link*> links;
    std::map<char, Link*> opponentLinks;
    std::vector<char> abilities;
    int downloadedData;
    int downloadedVirus;
    bool allowTwoAbilities;

public:
    Player();
    int getDataDownloadCount();
    int getVirusDownloadCount();
    void playAbility(int abilityInd, int target);
    void chooseAbility(std::string abilities);
};

#endif

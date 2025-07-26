#ifndef GAMESETUP_H
#define GAMESETUP_H

#include <string>
#include <vector>

struct GameSetupConfig {
    //default ability ordering: Link boost, Firewall, Download, Scan, Polarize
    std::string player1Abilities = "LFDSP";  
    std::string player2Abilities = "LFDSP";
    std::string player1LinkFile = "";
    std::string player2LinkFile = "";
    bool graphicsEnabled = false;
    
    //parsed link configurations (if files provided)
    std::vector<std::string> player1Links;
    std::vector<std::string> player2Links;
};

class GameSetup {
private:
    GameSetupConfig config;
    
    bool parseAbilityString(const std::string& abilities);
    bool loadLinksFromFile(const std::string& filename, std::vector<std::string>& links);
    void generateRandomLinks(std::vector<std::string>& links);
    bool validateLinkConfiguration(const std::vector<std::string>& links);

public:
    GameSetup();
    bool parseCommandLine(int argc, char* argv[]);
    const GameSetupConfig& getConfig() const;
    bool initializeGame(class GameModel& model) const;
};

#endif

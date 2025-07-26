#include "gamesetup.h"
#include "gamemodel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <set>
using namespace std; 

GameSetup::GameSetup() {}

bool GameSetup::parseCommandLine(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {

        string arg = argv[i];
        //-ability1 <order> 
        if (arg == "-ability1" && i + 1 < argc) {
            config.player1Abilities = argv[++i];
            if (!parseAbilityString(config.player1Abilities)) {
                cerr << "Error: Invalid ability string for player 1: " 
                          << config.player1Abilities << endl;
                return false;
            }
        }
        else if (arg == "-ability2" && i + 1 < argc) {
            config.player2Abilities = argv[++i];
            if (!parseAbilityString(config.player2Abilities)) {
                cerr << "Error: Invalid ability string for player 2: " 
                          << config.player2Abilities << endl;
                return false;
            }
        }
        else if (arg == "-link1" && i + 1 < argc) {
            config.player1LinkFile = argv[++i];
            if (!loadLinksFromFile(config.player1LinkFile, config.player1Links)) {
                cerr << "Error: Could not load links from file: " 
                          << config.player1LinkFile << endl;
                return false;
            }
        }
        else if (arg == "-link2" && i + 1 < argc) {
            config.player2LinkFile = argv[++i];
            if (!loadLinksFromFile(config.player2LinkFile, config.player2Links)) {
                cerr << "Error: Could not load links from file: " 
                          << config.player2LinkFile << endl;
                return false;
            }
        }
        else if (arg == "-graphics") {
            config.graphicsEnabled = true;
        }
        else {
            cerr << "Error: Unknown argument: " << arg << endl;
            return false;
        }
    }
    
    //generate random links for players that don't have link files
    if (config.player1Links.empty()) {
        generateRandomLinks(config.player1Links);
    }
    if (config.player2Links.empty()) {
        generateRandomLinks(config.player2Links);
    }
    
    return true;
}

bool GameSetup::parseAbilityString(const string& abilities) {
    //only 5 abilities
    if (abilities.length() != 5) {
        return false;
    }
    //check for valid ability letters
    set<char> validAbilities = {'L', 'F', 'D', 'S', 'P', 'E', 'G', 'H'};
    for (char c : abilities) {
        if (validAbilities.find(c) == validAbilities.end()) {
            return false;
        }
    }
    return true;
}

bool GameSetup::loadLinksFromFile(const string& filename, vector<string>& links) {
    //failed to open file
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    links.clear();
    string token;
    while (file >> token && links.size() < 8) {
        links.push_back(token);
    }
    file.close();
    
    if (links.size() != 8) {
        cerr << "Error: Link file must contain exactly 8 links, found " 
                  << links.size() << endl;
        return false;
    }
    
    return validateLinkConfiguration(links);
}

void GameSetup::generateRandomLinks(vector<string>& links) {
    //each player gets: V1, V2, V3, V4, D1, D2, D3, D4
    links = {"V1", "V2", "V3", "V4", "D1", "D2", "D3", "D4"};
    
    //shuffle the order randomly
    random_device rd;
    mt19937 g(rd());
    shuffle(links.begin(), links.end(), g);
}

bool GameSetup::validateLinkConfiguration(const vector<string>& links) {
    if (links.size() != 8) {
        return false;
    }
    
    set<string> expectedLinks = {"V1", "V2", "V3", "V4", "D1", "D2", "D3", "D4"};
    set<string> actualLinks(links.begin(), links.end());
    if (expectedLinks != actualLinks) {
        cerr << "Error: Link configuration must contain exactly V1, V2, V3, V4, D1, D2, D3, D4" 
                  << endl;
        return false;
    }
    return true;
}

const GameSetupConfig& GameSetup::getConfig() const {
    return config;
}

bool GameSetup::initializeGame(GameModel& model) const {
    // initialize players with abilities and links
    model.initializePlayers(config.player1Abilities, config.player2Abilities, config.player1Links, config.player2Links);
    model.placeLinksOnBoard();
    
    std::cout << "Game initialized successfully!\n";
    std::cout << "Player 1 abilities: " << config.player1Abilities << "\n";
    std::cout << "Player 2 abilities: " << config.player2Abilities << "\n";
    
    if (config.graphicsEnabled) {
        std::cout << "Graphics mode enabled.\n";
    }
    
    return true;
}

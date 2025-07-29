#include "gamecontroller.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>

GameController::GameController(GameModel& model) : model(model) {}

void GameController::play() {
    std::string line;
    std::cout << "\nRAIInet Game Started!\n";
    std::cout << "Available commands: move, abilities, ability, board, sequence, quit\n";
    std::cout << "Player " << model.getCurrentPlayer()->getId() << "'s turn > ";    
    
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        
        if (command == "quit" || line.empty()) {
            std::cout << "Game ended.\n";
            writeCommandHistory();
            break;
        }
        else if (command == "move") {
            std::string linkId, direction;
            if (iss >> linkId >> direction) {
                handleMove(linkId[0], direction, line);
            } else {
                std::cout << "Usage: move <link> <direction>\n";
                std::cout << "Example: move a up\n";
            }
        }
        else if (command == "abilities") {
            handleAbilities();
        }
        else if (command == "ability") {
            int abilityId;
            if (iss >> abilityId) {
                // Parse additional parameters for abilities that need them
                std::vector<std::string> args;
                std::string arg;
                while (iss >> arg) {
                    args.push_back(arg);
                }
                handleAbility(abilityId, args, line);
            } else {
                std::cout << "Usage: ability <id> [additional parameters]\n";
            }
        }
        else if (command == "board") {
            handleBoard();
        }
        else if (command == "sequence") {
            std::string filename;
            if (iss >> filename) {
                if (!handleSequence(filename)) {
                    break; // quit if handleSequence signals game over
                }
            } else {
                std::cout << "Usage: sequence <filename>\n";
            }
        }
        else {
            std::cout << "Unknown command: " << command << "\n";
            std::cout << "Available commands: move, abilities, ability, board, sequence, quit\n";
        }
        
        if (!model.isGameOver()) {
            std::cout << "Player " << model.getCurrentPlayer()->getId() << "'s turn > ";
        } else {
            std::cout << "Game Over!\n";
            int winner = model.getWinnerId();
            if (winner) {
                Player* p1 = model.getPlayer(1);
                Player* p2 = model.getPlayer(2);
                std::cout << "Player " << winner << " wins!\n";
                std::cout << "Player 1: " << p1->getDataDownloadCount() << "D, " << p1->getVirusDownloadCount() << "V\n";
                std::cout << "Player 2: " << p2->getDataDownloadCount() << "D, " << p2->getVirusDownloadCount() << "V\n";
            }
            writeCommandHistory();
            break;
        }
    }
}

bool GameController::handleMove(char linkId, const std::string& direction, const std::string& originalLine) {
    int dir = -1;
    if (direction == "up") dir = 0;
    else if (direction == "down") dir = 1;
    else if (direction == "left") dir = 2;
    else if (direction == "right") dir = 3;
    else {
        std::cout << "Invalid direction. Use: up, down, left, right\n";
        return false;
    }
    try {
        model.moveLink(model.getCurrentPlayer(), linkId, dir);
        std::cout << "Player " << model.getCurrentPlayer()->getId() << " moved link " << linkId << " " << direction << "\n";
        model.nextTurn();
        commandHistory.push_back(originalLine); // Only push if successful
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error moving link: " << e.what() << "\n";
        return false;
    }
}

std::string GameController::abilityFullName(char id) {
    switch (id) {
        case 'L': return "LinkBoost";
        case 'F': return "Firewall";
        case 'D': return "Download";
        case 'S': return "Scan";
        case 'P': return "Polarize";
        case 'E': return "Exchange";
        case 'G': return "GoLater";
        case 'H': return "Hijack";
        default:  return "Unknown";
    }
}

void GameController::handleAbilities() {
    Player* player = model.getCurrentPlayer();
    if (!player) {
        std::cout << "Error: No current player.\n";
        return;
    }
    const auto& abilities = player->getAbilities();
    std::cout << "Abilities for Player " << player->getId() << ":\n";
    for (size_t i = 0; i < abilities.size(); ++i) {
        const Ability* ability = abilities[i];
        std::string status = ability->isUsed() ? "(used)" : "(not used)";
        std::cout << "  " << (i+1) << ". " << abilityFullName(ability->getID()) << " " << status << "\n";
    }
}

bool GameController::handleAbility(int abilityId, const std::vector<std::string>& args, const std::string& originalLine) {
    try {
        model.useAbility(abilityId, args);

        // get ability name
        Player* player = model.getCurrentPlayer();
        const auto& abilities = player->getAbilities();
        std::string abilityName = (abilityId >= 1 && abilityId <= (int)abilities.size())
            ? abilityFullName(abilities[abilityId - 1]->getID())
            : "Unknown";

        // format arguments
        std::ostringstream argStream;
        for (size_t i = 0; i < args.size(); ++i) {
            argStream << args[i];
            if (i + 1 < args.size()) argStream << " ";
        }

        std::cout << "Player " << player->getId()
                  << " used the " << abilityName
                  << " ability in slot " << abilityId << ".\n";
        std::cout << formatAbilityArgs(abilityName, args) << "\n";

        commandHistory.push_back(originalLine); // only push if successful
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error using ability! " << e.what() << "\n";
        return false;
    }
}

void GameController::handleBoard() {
    model.notify(ChangeEvent::PrintBoard);
}

bool GameController::handleSequence(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file: " << filename << "\n";
        return true;
    }
    
    std::string line;
    std::cout << "Executing commands from " << filename << ":\n";
    
    while (std::getline(file, line)) {
        //skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;  
        
        std::cout << "> " << line << "\n";
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        
        if (command == "quit") {
            std::cout << "Game ended.\n";
            writeCommandHistory();
            file.close();
            return false;
        } else if (command == "move") {
            std::string linkId, direction;
            if (iss >> linkId >> direction) {
                handleMove(linkId[0], direction, line);
            } else {
                std::cout << "Usage: move <link> <direction>\n";
                std::cout << "Example: move a up\n";
            }
        } else if (command == "abilities") {
            handleAbilities();
        } else if (command == "ability") {
            int abilityId;
            if (iss >> abilityId) {
                // parse additional parameters for abilities that need them
                std::vector<std::string> args;
                std::string arg;
                while (iss >> arg) {
                    args.push_back(arg);
                }
                handleAbility(abilityId, args, line);
            } else {
                std::cout << "Usage: ability <id> [additional parameters]\n";
            }
        } else if (command == "board"){
            handleBoard();
        // recursively also handle sequence to other files
        } else if (command == "sequence") {
            std::string filename;
            if (iss >> filename) {
                handleSequence(filename);
            } else {
                std::cout << "Usage: sequence <filename>\n";
            }
        } else {
            std::cout << "Unknown command: " << command << "\n";
            std::cout << "Available commands: move, abilities, ability, board, sequence, quit\n";
        }
        
        if (model.isGameOver()) {
            std::cout << "Game ended during sequence execution.\n";
            return false;
        }
    }
    
    file.close();
    return true;
}

void GameController::notify(ChangeEvent event) {
    //handle any controller-specific responses to game events
    switch (event) {
        case ChangeEvent::GameOver:
            std::cout << "Game Over detected by controller!\n";
            break;
        default:
            break;
    }
}

void GameController::writeCommandHistory() {
    std::string filename = "tests/history.txt";
    std::ofstream out(filename);
    for (const auto& cmd : commandHistory) {
        out << cmd << std::endl;
    }
    std::cout << "Command history written to " << filename << std::endl;
}

std::string GameController::formatAbilityArgs(const std::string& abilityName, const std::vector<std::string>& args) {
    std::ostringstream out;
    if (abilityName == "Firewall" && args.size() == 2) {
        out << "Targeted cell: row " << args[0] << ", column " << args[1];
    } else if (abilityName == "Exchange" && args.size() == 2) {
        out << "Targeted links: " << args[0] << ", " << args[1];
    } else if (abilityName == "Hijack" && args.size() == 2) {
        out << "Targeted link: " << args[0] << "  |  Move direction: " << args[1];
    } else if (abilityName == "Download" && args.size() == 1) {
        out << "Targeted link: " << args[0];
    } else if (abilityName == "Scan" && args.size() == 1) {
        out << "Targeted link: " << args[0];
    } else if (abilityName == "Polarize" && args.size() == 1) {
        out << "Targeted link: " << args[0];
    } else if (abilityName == "LinkBoost" && args.size() == 1) {
        out << "Targeted link: " << args[0];
    } else if (abilityName == "GoLater" && args.empty()) {
        out << "(No arguments needed)";
    } else {
        // fallback: just print the arguments
        out << "Arguments: [";
        for (size_t i = 0; i < args.size(); ++i) {
            out << args[i];
            if (i + 1 < args.size()) out << " ";
        }
        out << "]";
    }
    return out.str();
}

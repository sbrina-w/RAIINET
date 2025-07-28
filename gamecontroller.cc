#include "gamecontroller.h"
#include <iostream>
#include <sstream>
#include <fstream>

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
            break;
        }
        else if (command == "move") {
            std::string linkId, direction;
            if (iss >> linkId >> direction) {
                handleMove(linkId[0], direction);
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
                handleAbility(abilityId, args);
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
                handleSequence(filename);
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
            break;
        }
    }
}

void GameController::handleMove(char linkId, const std::string& direction) {
    int dir = -1;
    
    if (direction == "up") dir = 0;
    else if (direction == "down") dir = 1;
    else if (direction == "left") dir = 2;
    else if (direction == "right") dir = 3;
    else {
        std::cout << "Invalid direction. Use: up, down, left, right\n";
        return;
    }
    
    try {
        model.moveLink(model.getCurrentPlayer(), linkId, dir);
        std::cout << "Player " << model.getCurrentPlayer()->getId() << " moved link " << linkId << " " << direction << "\n";
        // moveLink(...) already triggers the LinkMoved changeevent, calling TurnEnded would double printing
        // increment turn counter
        model.nextTurn();
    } catch (const std::exception& e) {
        std::cout << "Error moving link: " << e.what() << "\n";
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

void GameController::handleAbility(int abilityId, const std::vector<std::string>& args) {
    try {
        model.useAbility(abilityId, args);
        std::cout << "Player " << model.getCurrentPlayer()->getId() << " Used ability " << abilityId << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error using ability! " << e.what() << "\n";
    }
}

void GameController::handleBoard() {
    model.notify(ChangeEvent::PrintBoard);
}

void GameController::handleSequence(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file: " << filename << "\n";
        return;
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
            break;
        } else if (command == "move") {
            std::string linkId, direction;
            if (iss >> linkId >> direction) {
                handleMove(linkId[0], direction);
            } else {
                std::cout << "Usage: move <link> <direction>\n";
                std::cout << "Example: move a up\n";
            }
        } else if (command == "abilities") {
            handleAbilities();
        } else if (command == "ability") {
            int abilityId;
            if (iss >> abilityId) {
                // Parse additional parameters for abilities that need them
                std::vector<std::string> args;
                std::string arg;
                while (iss >> arg) {
                    args.push_back(arg);
                }
                handleAbility(abilityId, args);
            } else {
                std::cout << "Usage: ability <id> [additional parameters]\n";
            }
        } else if (command == "board"){
            handleBoard();
        //recursively also handle sequence to other files
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
            break;
        }
    }
    
    file.close();
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

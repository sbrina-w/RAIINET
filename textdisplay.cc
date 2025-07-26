#include "textdisplay.h"
#include "gamemodel.h"
#include "cell.h"
#include "celltype.h"
#include "link.h"
#include "player.h"
#include "linktype.h"

#include <iostream>
#include <iomanip>

void TextDisplay::notify(GameModel &model, ChangeEvent event)
{
    // Redraw on any board‐changing or turn‐ending event
    switch (event)
    {
    case ChangeEvent::GameStart:
    case ChangeEvent::LinkMoved:
    case ChangeEvent::DownloadOccurred:
    case ChangeEvent::AbilityUsed:
    case ChangeEvent::TurnEnded:
    {
        const Board &board = model.getBoard();
        Player* viewer = model.getPlayer(viewerId);
        Player* opponent = model.getPlayer(viewerId == 1 ? 2 : 1);

        if (!viewer || !opponent) return;

        //display player info
        std::cout << "Player " << viewerId << ":\n";
        std::cout << "Downloaded: " << viewer->getDataDownloadCount() << "D, " 
                  << viewer->getVirusDownloadCount() << "V\n";
        std::cout << "Abilities: " << viewer->getUnusedAbilityCount() << "\n";

        //display this player's links with their actual values
        const auto& myLinks = viewer->getLinks();
        for (const auto& pair : myLinks) {
            char id = pair.first;
            Link* link = pair.second;
            char typeChar = (link->getType() == LinkType::Virus) ? 'V' : 'D';
            std::cout << id << ": " << typeChar << link->getStrength() << " ";
        }
        std::cout << "\n";

        //display board
        std::cout << "========\n";
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                const Cell &cell = board.at(r, c);

                // 1) Server ports
                if (cell.getCellType() == CellType::ServerPort) {
                    std::cout << 'S';
                }
                //check for links
                else if (cell.getLink()) {
                    Link* link = cell.getLink();
                    std::cout << link->getId();
                }
                //empty space
                else {
                    std::cout << '.';
                }
            }
            std::cout << '\n';
        }
        std::cout << "========\n";

        //display opponent info
        std::cout << "Player " << (viewerId == 1 ? 2 : 1) << ":\n";
        std::cout << "Downloaded: " << opponent->getDataDownloadCount() << "D, " 
                  << opponent->getVirusDownloadCount() << "V\n";
        std::cout << "Abilities: " << opponent->getUnusedAbilityCount() << "\n";

        //display opponent's links (known information only)
        const auto& oppLinks = opponent->getLinks();
        for (const auto& pair : oppLinks) {
            char id = pair.first;
            Link* link = pair.second;
            
            std::cout << id << ": ";
            
            //check if we know this opponent link
            if (viewer->knowsOpponentLink(id) || link->isRevealed()) {
                char typeChar = (link->getType() == LinkType::Virus) ? 'V' : 'D';
                std::cout << typeChar << link->getStrength();
            } else {
                std::cout << "?";
            }
            std::cout << " ";
        }
        std::cout << "\n\n";
        
        break;
    }

    case ChangeEvent::GameOver:
        std::cout << "=== GAME OVER ===\n";
        break;

    default:
        // ignore other events
        break;
    }
}

#include "textdisplay.h"
#include "gamemodel.h"
#include "cell.h"
#include "celltype.h"
#include "link.h"
#include "player.h"
#include "linktype.h"

#include <iostream>
#include <iomanip>

inline const char *to_string(ChangeEvent e)
{
    switch (e)
    {
    case ChangeEvent::GameStart:
        return "GameStart";
    case ChangeEvent::LinkMoved:
        return "LinkMoved";
    case ChangeEvent::AbilityUsed:
        return "AbilityUsed";
    case ChangeEvent::DownloadOccurred:
        return "DownloadOccurred";
    case ChangeEvent::TurnEnded:
        return "TurnEnded";
    case ChangeEvent::PrintBoard:
        return "PrintBoard";
    case ChangeEvent::GameOver:
        return "GameOver";
    default:
        return "UnknownEvent";
    }
}

void TextDisplay::notify(GameModel &model, ChangeEvent event)
{
    switch (event)
    {
    case ChangeEvent::GameStart:
    case ChangeEvent::PrintBoard:
    // case ChangeEvent::LinkMoved: //<-- removed because this always leads to a turnended
    // case ChangeEvent::DownloadOccurred: //removed erase as well
    case ChangeEvent::AbilityUsed:
    case ChangeEvent::TurnEnded:
    {
        // for future debugging:
        // std::cout << "[DEBUG] event = " << to_string(event) << " for TextDisplay" << viewerId << "\n";

        const Board &board = model.getBoard();
        Player *current = model.getCurrentPlayer();

        // only print for the current player since both text displays will get this notification
        if (current->getId() != viewerId)
        {
            return;
        }

        Player *p1 = model.getPlayer(1);
        Player *p2 = model.getPlayer(2);
        if (!current || !p1 || !p2)
            return;

        // for turn ended, print a turn ended break.
        if (event == ChangeEvent::TurnEnded)
        {
            std::cout << "\n"
                      << "------" << " PLAYER " << (current->getId() == 1 ? "2" : "1") << " TURN ENDED ------" << "\n";
        }

        if (event == ChangeEvent::GameStart || event == ChangeEvent::TurnEnded)
        {
            std::cout << "\n"
                      << "------" << " PLAYER " << current->getId() << " TURN START ------" << "\n";
        }

        // prints player 1's info (player 1 always on top)
        std::cout << "\n"
                  << "Player 1";
        if (current->getId() == 1)
        {
            std::cout << " (Current Player)";
        }
        std::cout << ":\n";
        std::cout << "Downloaded: "
                  << p1->getDataDownloadCount() << "D, "
                  << p1->getVirusDownloadCount() << "V\n";
        std::cout << "Abilities: "
                  << p1->getUnusedAbilityCount() << "\n";
        // only show the full details if it playuer1's turn (current==1) or if revealed/remembered
        for (auto const &[id, link] : p1->getLinks())
        {
            std::cout << id << ": ";
            if (current->getId() == 1 || current->knowsOpponentLink(id) || link->isRevealed())
            {
                char t = (link->getType() == LinkType::Virus ? 'V' : 'D');
                std::cout << t << link->getStrength();
            }
            else
            {
                std::cout << "?";
            }
            std::cout << " ";
        }
        std::cout << "\n";

        // to print the board (player 1's links always on topm, player 2 at the bottom)
        std::cout << "========\n";
        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                const Cell &cell = board.at(r, c);

                if (cell.getCellType() == CellType::ServerPort)
                {
                    std::cout << 'S';
                }
                else if (Link *link = cell.getLink())
                {
                    std::cout << link->getId();
                }
                else if (cell.getCellType() == CellType::Firewall)
                {
                    Player *owner = cell.getFirewallOwner();
                    std::cout << (owner
                                      ? (owner->getId() == 1 ? 'm' : 'w')
                                      : '.');
                }
                else
                {
                    std::cout << '.';
                }
            }
            std::cout << '\n';
        }
        std::cout << "========\n";

        // print PLAYER 2's information
        std::cout << "Player 2";
        if (current->getId() == 2)
        {
            std::cout << " (Current Player)";
        }
        std::cout << ":\n";
        std::cout << "Downloaded: "
                  << p2->getDataDownloadCount() << "D, "
                  << p2->getVirusDownloadCount() << "V\n";
        std::cout << "Abilities: "
                  << p2->getUnusedAbilityCount() << "\n";
        for (auto const &[id, link] : p2->getLinks())
        {
            std::cout << id << ": ";
            // only show link info if current player is player 2 OR if link's are revealed
            if (current->getId() == 2 || current->knowsOpponentLink(id) || link->isRevealed())
            {
                char t = (link->getType() == LinkType::Virus ? 'V' : 'D');
                std::cout << t << link->getStrength();
            }
            else
            {
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
        // ignore everything else
        break;
    }
}

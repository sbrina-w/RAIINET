#include "textdisplay.h"
#include "gamemodel.h"
#include "cell.h"
#include "celltype.h"

#include "link.h"
#include "player.h"

#include <iostream>

void TextDisplay::notify(GameModel &model, ChangeEvent event)
{
    // Redraw on any board‐changing or turn‐ending event
    switch (event)
    {
    case ChangeEvent::LinkMoved:
    case ChangeEvent::DownloadOccurred:
    case ChangeEvent::AbilityUsed:
    case ChangeEvent::TurnEnded:
    {
        const Board &board = model.getBoard();
        int viewer = viewerId;

        std::cout << "\n=== Player " << viewer << " view ===\n";
        for (int r = 0; r < 8; ++r)
        {
            for (int c = 0; c < 8; ++c)
            {
                const Cell &cell = board.at(r, c);

                // 1) Server ports
                if (cell.getCellType() == CellType::ServerPort)
                {
                    std::cout << "[S]";
                }
                // 2) Empty normal cell
                else if (!cell.getLink())
                {
                    std::cout << "[ ]";
                }
                // 3) Occupied by a link
                else
                {
                    auto *link = cell.getLink();
                    // if it belongs to the viewing player, or it’s already been revealed:
                    if (link->getOwner()->getId() == viewer || link->isRevealed())
                    {
                        std::cout << '[' << link->getId() << ']';
                    }
                    else
                    {
                        std::cout << "[?]";
                    }
                }
            }
            std::cout << '\n';
        }
        break;
    }

    case ChangeEvent::GameOver:
        // You can handle end‐of‐game messaging here if desired.
        break;

    default:
        // ignore other events
        break;
    }
}

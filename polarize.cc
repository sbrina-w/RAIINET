#include "polarize.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <stdexcept>
using namespace std;

Polarize::Polarize() : Ability('P') {}

// args[0]: link id (char, e.g. "A" or "a")
// Usage: ability <N> <linkID>
void Polarize::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("Polarize: missing linkID");
    char linkID = args[0][0];

    Link* link = model.findLinkById(linkID);
    if (!link) throw invalid_argument("Polarize: link not found");
    if (!model.isLinkOnBoard(link)) throw invalid_argument("Polarize: link is not currently on the board");

    //find the link's position
    Board& board = model.getBoard();
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            if (board.at(r, c).getLink() == link) {
                // Switch the link's type
                link->switchType();
                markUsed();

                // Check for firewall effect after polarization
                Cell& cell = board.at(r, c);
                if (cell.getCellType() == CellType::Firewall) {
                    Player* firewallOwner = cell.getFirewallOwner();
                    if (firewallOwner && firewallOwner != link->getOwner() && link->getType() == LinkType::Virus) {
                        // Reveal the link and let the firewall owner learn it
                        // should have already happened, since the cell had to previously move onto the firewall
                        // do this just to be safe and consistent (similar pattern for exchange)
                        link->reveal();
                        firewallOwner->learnOpponentLink(link->getId(), link);
                        // Download the link for its owner and remove from board
                        link->getOwner()->incrementDownload(LinkType::Virus);
                        cell.removeLink();
                        model.clearChangedCells();
                        model.addChangedCell(r, c);
                        model.notify(ChangeEvent::DownloadOccurred);
                        return;
                    }
                }
                
                //mark cell as changed for graphics update
                model.clearChangedCells();
                model.addChangedCell(r, c);
                return;
            }
        }
    }
    
    throw runtime_error("Polarize: could not find link on board");
}

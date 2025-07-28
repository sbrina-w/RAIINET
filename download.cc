#include "download.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <stdexcept>
using namespace std;

Download::Download() : Ability('D') {}

// args[0]: link id (char, e.g. "A" or "a")
// Usage: ability <N> <linkID>
void Download::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("Download: missing linkID");
    char linkID = args[0][0];

    Player* current = model.getCurrentPlayer();
    Link* link = model.findLinkById(linkID);

    if (!link) throw invalid_argument("Download: link not found");
    if (!model.isLinkOnBoard(link)) throw invalid_argument("Download: link is not currently on the board");
    if (link->getOwner() == current) throw invalid_argument("Download: must target an opponent's link, not your own");

    // Find the link's position on the board and remove it
    // Just do a scan of the board for now, don't have to set row + col fields inside of Link
    Board& board = model.getBoard();
    bool removed = false;
    for (int r = 0; r < 8 && !removed; ++r) {
        for (int c = 0; c < 8 && !removed; ++c) {
            if (board.at(r, c).getLink() == link) {
                board.at(r, c).removeLink();
                model.clearChangedCells();
                model.addChangedCell(r, c);
                removed = true;
            }
        }
    }

    // Increment download count for the current player
    current->incrementDownload(link->getType());
    // Learn the downloaded link
    current->learnOpponentLink(linkID, link);

    markUsed();
}

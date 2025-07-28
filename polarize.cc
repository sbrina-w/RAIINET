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
                
                //mark cell as changed for graphics update
                model.clearChangedCells();
                model.addChangedCell(r, c);
                return;
            }
        }
    }
    
    throw runtime_error("Polarize: could not find link on board");
}

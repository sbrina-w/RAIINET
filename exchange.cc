#include "exchange.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include "board.h"
#include <stdexcept>
using namespace std;

Exchange::Exchange() : Ability('E') {}

// args[0]: link id 1 (char, e.g. "a")
// args[1]: link id 2 (char, e.g. "b")
// Usage: ability <N> <linkID1> <linkID2>
void Exchange::execute(GameModel& model, vector<string> args) {
    if (args.size() < 2) throw invalid_argument("Exchange: missing linkIDs");
    char linkID1 = args[0][0];
    char linkID2 = args[1][0];

    if (linkID1 == linkID2) throw invalid_argument("Exchange: cannot exchange the same link with itself");

    Player* player = model.getCurrentPlayer();
    Link* link1 = player->getLink(linkID1);
    Link* link2 = player->getLink(linkID2);

    if (!link1 || !link2) throw invalid_argument("Exchange: both links must exist and be owned by the current player");
    if (!model.isLinkOnBoard(link1) || !model.isLinkOnBoard(link2))
        throw invalid_argument("Exchange: both links must be on the board");

    Board& board = model.getBoard();
    int r1 = -1, c1 = -1, r2 = -1, c2 = -1;
    // Find positions of both links
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board.at(r, c).getLink() == link1) {
                r1 = r; c1 = c;
            }
            if (board.at(r, c).getLink() == link2) {
                r2 = r; c2 = c;
            }
        }
    }
    if (r1 == -1 || r2 == -1)
        throw runtime_error("Exchange: could not find both links on the board");

    // Swap the links
    board.at(r1, c1).setLink(link2);
    board.at(r2, c2).setLink(link1);

    markUsed();
    
    //mark both cells as changed
    model.clearChangedCells();
    model.addChangedCell(r1, c1);
    model.addChangedCell(r2, c2);
}

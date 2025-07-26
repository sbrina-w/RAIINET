#include "exchange.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include "board.h"
#include <stdexcept>
using namespace std;

// args[0]: link id 1 (char, e.g. "a")
// args[1]: link id 2 (char, e.g. "b")
// Usage: ability <N> <linkID1> <linkID2>
void Exchange::execute(GameModel& model, vector<string> args) {
    if (args.size() < 2) throw invalid_argument("Exchange: missing linkIDs");
    char linkID1 = args[0][0];
    char linkID2 = args[1][0];

    // TODO: Find both links and ensure both belong to current player
    // Link* link1 = player->getLink(linkID1);
    // Link* link2 = player->getLink(linkID2);
    // if (!link1 || !link2) throw invalid_argument("Exchange: links not found or not owned by player");

    // TODO: Swap their positions on the board
    // model.swapLinks(link1, link2);
    // used = true;
}

#include "linkboost.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <stdexcept>
using namespace std;

LinkBoost::LinkBoost() : Ability('L') {}

// args[0]: link id (char, e.g. "a" or "A")
// Usage: ability <N> <linkID>
void LinkBoost::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("LinkBoost: missing linkID in parameters");
    char linkID = args[0][0];

    // 1. Get the current player
    Player* player = model.getCurrentPlayer();
    if (!player) throw runtime_error("LinkBoost: could not get current player");

    // 2. Get the link owned by the player
    Link* link = player->getLink(linkID);
    if (!link) throw invalid_argument("LinkBoost: link not found or not owned by player");

    // 3. Check that the link is on the board
    if (!model.isLinkOnBoard(link)) throw invalid_argument("LinkBoost: link is not currently on the board");

    // 4. Set the link as boosted
    link->setBoosted(true);

    // 5. Mark this ability as used
    markUsed();
}

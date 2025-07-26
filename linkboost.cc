#include "linkboost.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <stdexcept>
using namespace std;

// args[0]: link id (char, e.g. "a" or "A")
// Usage: ability <N> <linkID>
void LinkBoost::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("LinkBoost: missing linkID");
    char linkID = args[0][0];

    // Find the current player (TODO: depends on turn logic)
    // Player* player = model.getCurrentPlayer();
    // Link* link = player->getLink(linkID);
    // if (!link) throw invalid_argument("LinkBoost: link not found or not owned by player");

    // link->setBoosted(true);
    // TODO: Ensure movement logic in Board/GameModel checks for boosted status and moves 2 squares instead of 1.
    // Mark this ability as used
    // used = true;
}

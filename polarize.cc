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

    // TODO: Find the link (could be on either player)
    // Link* link = model.findLink(linkID);
    // if (!link) throw invalid_argument("Polarize: link not found");

    // link->switchType();
    // used = true;
}

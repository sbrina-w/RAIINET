#include "hijack.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <stdexcept>
using namespace std;

Hijack::Hijack() : Ability('H') {}

// args[0]: link id (char, opponent's link, e.g. "A")
// args[1]: direction (string, e.g. "up", "down", "left", "right")
// Usage: ability <N> <linkID> <direction>
void Hijack::execute(GameModel& model, vector<string> args) {
    if (args.size() < 2) throw invalid_argument("Hijack: missing linkID/direction");
    char linkID = args[0][0];
    string direction = args[1];

    // TODO: Find the opponent's link
    // Link* link = model.getOpponentPlayer()->getLink(linkID);
    // if (!link) throw invalid_argument("Hijack: link not found");

    // assume that model.moveLink already handles boosted vs unboosted link

    // model.moveLink(linkID, direction);
    // used = true;
}

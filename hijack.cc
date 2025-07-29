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
    if (args.size() > 2) throw invalid_argument("Hijack: too many arguments");
    char linkID = args[0][0];
    string direction = args[1];

    Player* current = model.getCurrentPlayer();
    Link* link = model.findLinkById(linkID);

    if (!link) throw invalid_argument("Hijack: link not found");
    Player* owner = link->getOwner();
    if (owner == current) throw invalid_argument("Hijack: can only hijack opponent's link");
    if (!model.isLinkOnBoard(link)) throw invalid_argument("Hijack: link is not currently on the board");

    int dir = -1;
    if (direction == "up") dir = 0;
    else if (direction == "down") dir = 1;
    else if (direction == "left") dir = 2;
    else if (direction == "right") dir = 3;
    else throw invalid_argument("Hijack: invalid direction (must be 'up', 'down', 'left', or 'right')");

    model.moveLink(owner, linkID, dir);

    markUsed();
}

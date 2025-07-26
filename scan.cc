#include "scan.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <iostream>
#include <stdexcept>
using namespace std;

// args[0]: link id (char, e.g. "A" or "a")
// Usage: ability <N> <linkID>
void Scan::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("Scan: missing linkID");
    char linkID = args[0][0];

    // TODO: Find the link (not owned by current player)
    // Link* link = model.findOpponentLink(linkID);
    // if (!link) throw invalid_argument("Scan: link not found or is your own link");

    // cout << "Link " << linkID << ": Type = " << (link->getType() == LinkType::Data ? "Data" : "Virus")
    //      << ", Strength = " << link->getStrength() << endl;
    // link->reveal(); // Optionally reveal for display
    // used = true;
}

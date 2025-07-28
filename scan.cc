#include "scan.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Scan::Scan() : Ability('S') {}

// args[0]: link id (char, e.g. "A" or "a")
// Usage: ability <N> <linkID>
void Scan::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("Scan: missing linkID");
    char linkID = args[0][0];

    Player* current = model.getCurrentPlayer();
    Link* link = model.findLinkById(linkID);

    if (!link) throw invalid_argument("Scan: link not found");
    if (!model.isLinkOnBoard(link)) throw invalid_argument("Scan: link is not currently on the board");
    if (link->getOwner() == current) throw invalid_argument("Scan: cannot scan your own link");

    // Reveal and print info
    link->reveal();
    cout << "Link " << linkID << ": Type = " 
         << (link->getType() == LinkType::Data ? "Data" : "Virus")
         << ", Strength = " << link->getStrength() << endl;

    // let the player remember this link
    current->learnOpponentLink(linkID, link);

    markUsed();
}

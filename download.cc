#include "download.h"
#include "gamemodel.h"
#include "player.h"
#include "link.h"
#include <stdexcept>
using namespace std;

// args[0]: link id (char, e.g. "A" or "a")
// Usage: ability <N> <linkID>
void Download::execute(GameModel& model, vector<string> args) {
    if (args.size() < 1) throw invalid_argument("Download: missing linkID");
    char linkID = args[0][0];

    // TODO: Find the opponent and their link
    // Player* opponent = model.getOpponentPlayer();
    // Link* link = opponent->getLink(linkID);
    // if (!link) throw invalid_argument("Download: link not found");

    // model.downloadLink(link); // This should handle updating download counts and removing the link from the board
    // used = true;
}

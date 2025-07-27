#include "golater.h"
#include "gamemodel.h"
#include "player.h"
using namespace std;

GoLater::GoLater() : Ability('G') {}

// No args required
// Usage: ability <N>
void GoLater::execute(GameModel& model, vector<string> args) {
    // TODO: Set a flag on the current player or model to allow 2 abilities next turn
    // model.getCurrentPlayer()->setGoLaterActive(true);
    // used = true;
}

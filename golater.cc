#include "golater.h"
#include "gamemodel.h"
#include "player.h"
using namespace std;

GoLater::GoLater() : Ability('G') {}

// No args required
// Usage: ability <N>
void GoLater::execute(GameModel& model, vector<string> args) {
    model.getCurrentPlayer()->activateGoLater();
    markUsed();
}

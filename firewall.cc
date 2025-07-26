#include "firewall.h"
#include "gamemodel.h"
#include "board.h"
#include "cell.h"
#include <stdexcept>
using namespace std;

// args[0]: row (string, needs to be converted to int)
// args[1]: col (string, needs to be converted to int)
// Usage: ability <N> <row> <col>
void Firewall::execute(GameModel& model, vector<string> args) {
    if (args.size() < 2) throw invalid_argument("Firewall: missing row/col");
    int row = stoi(args[0]);
    int col = stoi(args[1]);

    Board& board = model.getBoard();
    Cell& cell = board.at(row, col);

    // TODO: Check if cell is empty and not a server port
    // if (cell.getCellType() == CellType::ServerPort || cell.getLink())
    //     throw invalid_argument("Firewall: Cannot place on server port or occupied cell");

    // cell.setFirewallOwner(model.getCurrentPlayer());
    // cell.setCellType(CellType::Firewall);

    // TODO: Ensure Board/Cell logic handles firewall effects during movement and battles
    // used = true;
}

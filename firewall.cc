#include "firewall.h"
#include "gamemodel.h"
#include "board.h"
#include "cell.h"
#include "celltype.h"
#include <stdexcept>
using namespace std;

Firewall::Firewall() : Ability('F') {}

// args[0]: row (string, needs to be converted to int)
// args[1]: col (string, needs to be converted to int)
// Usage: ability <N> <row> <col>
void Firewall::execute(GameModel& model, vector<string> args) {
    if (args.size() < 2) throw invalid_argument("Firewall: missing row/col");
    int row = stoi(args[0]);
    int col = stoi(args[1]);

    // bounds check
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        throw invalid_argument("Firewall: row/col out of bounds");
    }

    Board& board = model.getBoard();
    Cell& cell = board.at(row, col);

    // Check if cell is valid for firewall placement
    if (cell.getCellType() == CellType::ServerPort)
        throw invalid_argument("Firewall: Cannot place on server port");
    if (cell.getLink())
        throw invalid_argument("Firewall: Cannot place on occupied cell");
    if (cell.getCellType() == CellType::Firewall)
        throw invalid_argument("Firewall: Cannot place on existing firewall");

    // Place the firewall
    cell.setFirewallOwner(model.getCurrentPlayer());
    cell.setCellType(CellType::Firewall);

    markUsed();
}

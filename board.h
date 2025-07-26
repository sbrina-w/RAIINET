#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "player.h"

class Board
{
private:
    Cell cells[8][8];

public:

    //move and placeFirewall will belong in GameModel instead

    Board();
    // Mark the two server‐port cells in row 0 and row 7
    // (and leave the rest as Normal)
    void initServerPorts();

    // for text display to get the cell at every point on the board
    Cell &at(int row, int col);
    const Cell &at(int row, int col) const;
};

#endif

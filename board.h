#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include "player.h"

class Board {
private:
    Cell cells[8][8];

public:
    Board();
    void move(char linkID, int dir);
    void placeFirewall(int row, int col, Player* owner);
};

#endif

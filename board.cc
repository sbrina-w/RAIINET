#include "board.h"
#include <stdexcept>

Board::Board() {}

void Board::initServerPorts()
{
    // row 0 ports:
    cells[0][3].setCellType(CellType::ServerPort);
    cells[0][4].setCellType(CellType::ServerPort);
    // row 7 ports:
    cells[7][3].setCellType(CellType::ServerPort);
    cells[7][4].setCellType(CellType::ServerPort);
}

// for non-const board
Cell &Board::at(int row, int col)
{
    //out of bounds check
    if (row < 0 || row > 7 || col < 0 || col > 7)
    {
        throw std::out_of_range("Board::at()");
    }
    return cells[row][col];
}

//for const board (re-use out of bounds logic from above)
const Cell &Board::at(int row, int col) const
{
    return const_cast<Board *>(this)->at(row, col);
}

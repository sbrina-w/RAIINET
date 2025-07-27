#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include <vector>
#include "celltype.h"
#include "observer.h"
#include "xwindow.h"

using namespace std;

class GraphicsDisplay : public Observer {
private:
    Xwindow* window;
    int viewerId;
    //track current drawn
    vector<vector<char>> lastSnapshot;

    //layout
    static const int CELL_SIZE = 60;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 100;
    static const int INFO_PANEL_HEIGHT = 80;
    
    //drawing methods
    void drawBoard(class GameModel& model);
    //redraw exactly one cell at (row,col)
    void drawCell(int row, int col, GameModel& model);
    void drawPlayerInfo(class GameModel& model);
    void drawGrid();
    int getLinkColor(class Link* link, bool isRevealed, bool isOwner) const;

public:
    GraphicsDisplay(int viewerId = 1);
    ~GraphicsDisplay();
    void notify(GameModel& model, ChangeEvent event) override;
    void setViewerId(int id) { viewerId = id; }
};

#endif

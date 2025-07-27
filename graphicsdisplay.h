#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "observer.h"
#include "xwindow.h"

class GraphicsDisplay : public Observer {
private:
    Xwindow* window;
    int viewerId;

    //layout
    static const int CELL_SIZE = 60;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 100;
    static const int INFO_PANEL_HEIGHT = 80;
    
    //drawing methods
    void drawBoard(class GameModel& model);
    void drawCell(int row, int col, char content, int color);
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

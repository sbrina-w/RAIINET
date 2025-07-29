#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include <vector>
#include <X11/Xlib.h>
#include <sstream>
#include <string>
#include "link.h"
#include "observer.h"
#include "xwindow.h"
#include "gamemodel.h"
#include "celltype.h"
#include <memory>

class GraphicsDisplay : public Observer {
public:
  explicit GraphicsDisplay(int viewerId = 1);
  ~GraphicsDisplay() override;

  void notify(GameModel& model, ChangeEvent event) override;
  void setViewerId(int id) { viewerId = id; }

private:
  std::unique_ptr<Xwindow> window;    //shared window
  int           viewerId;  //1 or 2
  Pixmap        buffer1;   //off-screen for player 1
  Pixmap        buffer2;   //off-screen for player 2
  bool buffer2Built = false;
  void updateBuffersWithChangedCells(const std::vector<std::pair<int,int>>& changedCells, GameModel& model);

  //layout constants
  static const int CELL_SIZE                = 60;
  static const int BOARD_OFFSET_X           = 50;
  static const int BOARD_OFFSET_Y           = 170;
  static const int BOARD_OFFSET_HEIGHT_Y    = 100;
  static const int INFO_PANEL_HEIGHT        = 80;

  //core draw routines:
  void drawGrid();
  void drawPlayerInfo(GameModel& model);
  void drawBoardToPixmap(Pixmap pix, GameModel& model);
  void drawCell(int row,int col,GameModel& model);
  int  getLinkColor(Link* link,bool isRevealed,bool isOwner) const;
};

#endif

#include "graphicsdisplay.h"
#include "gamemodel.h"
#include "linktype.h"
#include "link.h"
#include "player.h"
#include "board.h"
#include "cell.h"
#include "celltype.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

GraphicsDisplay::GraphicsDisplay(int viewerId)
  : viewerId(viewerId)
{
   int w = BOARD_OFFSET_X*2 + CELL_SIZE*8;
   int h = BOARD_OFFSET_Y*2 + CELL_SIZE*8 + INFO_PANEL_HEIGHT*2;
   window = new Xwindow(w, h);
   //allocate two pixmaps
   buffer1 = window->makePixmap();
   buffer2 = window->makePixmap();
  //prerender grid once into buffer1
  Drawable oldD = window->getDrawable();
  window->setDrawable(buffer1);
  drawGrid();
  window->setDrawable(oldD);
}

GraphicsDisplay::~GraphicsDisplay() {
  //free pixmaps before deleting window
  if (window) {
    XFreePixmap(window->getDisplay(), buffer1);
    XFreePixmap(window->getDisplay(), buffer2);
    delete window;
    window = nullptr;
  }
}

void GraphicsDisplay::notify(GameModel& model, ChangeEvent event) {
  //update view to current player
  int active = model.getCurrentPlayer()->getId();
  setViewerId(active);
  if (event != ChangeEvent::GameStart && viewerId != active) return;

  //GameStart: draw both buffers, show active
  if (event == ChangeEvent::GameStart) {
    //build only buffer1 (grid already there)
    drawPlayerInfo(model);
    drawBoardToPixmap(buffer1, model);
    //one‐shot blit + implicit flush
    window->copyPixmap(
      buffer1,
      0,0,
      window->getWidth(), window->getHeight(),
      0,0
    );
    buffer2Built = false;
    return;
  }

  //update all changed cells in both buffers, but each using its own perspective
  if (event == ChangeEvent::LinkMoved || event == ChangeEvent::DownloadOccurred || event == ChangeEvent::AbilityUsed) {
    const auto& changedCells = model.getChangedCells();
    if(!changedCells.empty()) {
      //minimal redraw in both buffers
      updateBuffersWithChangedCells(changedCells, model);
      //blit only the active buffer
      Pixmap show = (viewerId == 1 ? buffer1 : buffer2);
      window->copyPixmap(
        show,
        0, 0,
        window->getWidth(), window->getHeight(),
        0, 0
      );
    }
    return;
  }

  //TurnEnded: instant blit the other pixmap
  {
    //if switching to player 2 and buffer2 not built
    if (viewerId == 2 && !buffer2Built) {
      drawBoardToPixmap(buffer2, model);
      buffer2Built = true;
    } else {
      //update the active buffer's info panel
      Drawable oldD = window->getDrawable();
      window->setDrawable(viewerId == 1 ? buffer1 : buffer2);
      drawPlayerInfo(model);
      window->setDrawable(oldD);
    }

    //one-shot swap into window
    window->copyPixmap(
      (viewerId == 1 ? buffer1 : buffer2),
      0,0,
      window->getWidth(), window->getHeight(),
      0,0
    );
    return;
  }

  //GameOver
  if (event == ChangeEvent::GameOver) {
    window->drawString(
      window->getWidth()/2 - 50,
      window->getHeight()/2,
      "GAME OVER"
    );
  }
}

//draws grid into current drawable
void GraphicsDisplay::drawGrid() {
  for (int i = 0; i <= 8; ++i) {
    int x = BOARD_OFFSET_X + i*CELL_SIZE;
    window->drawLine(x, BOARD_OFFSET_Y,
                     x, BOARD_OFFSET_Y + 8*CELL_SIZE);
    int y = BOARD_OFFSET_Y + i*CELL_SIZE;
    window->drawLine(BOARD_OFFSET_X, y,
                     BOARD_OFFSET_X + 8*CELL_SIZE, y);
  }
}

//info panel (always on-screen)
void GraphicsDisplay::drawPlayerInfo(GameModel& model) {
  Player* viewer = model.getPlayer(viewerId);
  Player* opponent = model.getPlayer(viewerId == 1 ? 2 : 1);
  if (!viewer || !opponent) return;

  //clear the info areas
  window->fillRectangle(0, 0, window->getWidth(), INFO_PANEL_HEIGHT, Xwindow::White);
  window->fillRectangle(0, window->getHeight() - INFO_PANEL_HEIGHT, 
                       window->getWidth(), INFO_PANEL_HEIGHT, Xwindow::White);

  //display current player info (top)
  ostringstream s1;
  s1 << "Player " << viewerId << ":";
  window->drawString(10, 20, s1.str(), Xwindow::Black);
  
  ostringstream s2;
  s2 << "Downloaded: " << viewer->getDataDownloadCount() << "D, " 
     << viewer->getVirusDownloadCount() << "V";
  window->drawString(10, 35, s2.str(), Xwindow::Black);
  
  ostringstream s3;
  s3 << "Abilities: " << viewer->getUnusedAbilityCount();
  window->drawString(10, 50, s3.str(), Xwindow::Black);

  //display this player's links with their actual values
  ostringstream s4;
  const auto& myLinks = viewer->getLinks();
  for (const auto& pair : myLinks) {
    char id = pair.first;
    Link* link = pair.second;
    char typeChar = (link->getType() == LinkType::Virus) ? 'V' : 'D';
    s4 << id << ": " << typeChar << link->getStrength() << " ";
  }
  window->drawString(10, 65, s4.str(), Xwindow::Black);

  //display opponent info (bottom)
  ostringstream s5;
  s5 << "Player " << (viewerId == 1 ? 2 : 1) << ":";
  window->drawString(10, window->getHeight() - 65, s5.str(), Xwindow::Black);
  
  ostringstream s6;
  s6 << "Downloaded: " << opponent->getDataDownloadCount() << "D, " 
     << opponent->getVirusDownloadCount() << "V";
  window->drawString(10, window->getHeight() - 50, s6.str(), Xwindow::Black);
  
  ostringstream s7;
  s7 << "Abilities: " << opponent->getUnusedAbilityCount();
  window->drawString(10, window->getHeight() - 35, s7.str(), Xwindow::Black);

  //display opponent's links (known information only)
  ostringstream s8;
  const auto& oppLinks = opponent->getLinks();
  for (const auto& pair : oppLinks) {
    char id = pair.first;
    Link* link = pair.second;
    
    s8 << id << ": ";
    
    //check if we know this opponent link
    if (viewer->knowsOpponentLink(id) || link->isRevealed()) {
      char typeChar = (link->getType() == LinkType::Virus) ? 'V' : 'D';
      s8 << typeChar << link->getStrength();
    } else {
      s8 << "?";
    }
    s8 << " ";
  }
  window->drawString(10, window->getHeight() - 20, s8.str(), Xwindow::Black);
}

//full‐board draw into pixmap
void GraphicsDisplay::drawBoardToPixmap(Pixmap pix, GameModel& model) {
  Drawable old = window->getDrawable();
  window->setDrawable(pix);
  window->clear();
  drawGrid();
  drawPlayerInfo(model);
  for (int r = 0; r < 8; ++r)
    for (int c = 0; c < 8; ++c)
      drawCell(r,c,model);
  window->setDrawable(old);
}

//draw one cell at (row,col)
void GraphicsDisplay::drawCell(int row,int col,GameModel& model) {
  const Board& board = model.getBoard();
  const Cell&  cell  = board.at(row,col);
  int x = BOARD_OFFSET_X + col*CELL_SIZE;
  int y = BOARD_OFFSET_Y + row*CELL_SIZE;

  window->fillRectangle(x+1,y+1,CELL_SIZE-2,CELL_SIZE-2,Xwindow::White);

  if (cell.getCellType() == CellType::ServerPort) {
    window->fillRectangle(x+5,y+5,CELL_SIZE-10,CELL_SIZE-10,Xwindow::Blue);
    window->drawString(x+CELL_SIZE/2-5,y+CELL_SIZE/2+5,"S",Xwindow::White);
  }
  else if (cell.getLink()) {
    Link* link = cell.getLink();
    bool isO   = link->getOwner()->getId()==viewerId;
    bool rev   = link->isRevealed()||isO;
    int  bg    = getLinkColor(link,rev,isO);
    window->fillRectangle(x+5,y+5,CELL_SIZE-10,CELL_SIZE-10,bg);
    string id(1,link->getId());
    window->drawString(x+CELL_SIZE/2-5,y+CELL_SIZE/2+5,id,Xwindow::White);
  }
  if (cell.getCellType() == CellType::Firewall) {
    Player* o = cell.getFirewallOwner();
    if (o) {
      char f = (o->getId()==1?'m':'w');
      window->drawString(x+5,y+CELL_SIZE-15,string(1,f),Xwindow::Red);
    }
  }

  //cell border
  window->drawLine(x,   y,   x+CELL_SIZE, y,   Xwindow::Black);
  window->drawLine(x,   y,   x,           y+CELL_SIZE, Xwindow::Black);
  window->drawLine(x+CELL_SIZE,y,   x+CELL_SIZE,y+CELL_SIZE,Xwindow::Black);
  window->drawLine(x,   y+CELL_SIZE,x+CELL_SIZE,y+CELL_SIZE,Xwindow::Black);
}

//colour selection
int GraphicsDisplay::getLinkColor(Link* link,bool isRevealed,bool isOwner) const {
  if (!isRevealed && !isOwner) return Xwindow::Black;
  return link->getType()==LinkType::Virus ? Xwindow::Red : Xwindow::Green;
}

//helper for redrawing changed cells
void GraphicsDisplay::updateBuffersWithChangedCells(
    const vector<pair<int,int>>& changedCells,
    GameModel& model)
{
    //remember active
    int activeViewer = viewerId;
    //update both buffers
    for (int pid = 1; pid <= 2; ++pid) {
        //set persepctive to pid
        setViewerId(pid);
        Pixmap pix = (pid==1?buffer1:buffer2);
        auto oldD = window->getDrawable();
        window->setDrawable(pix);

        //redraw info panel for current perspective
        drawPlayerInfo(model);

        //redraw only changed cells
        for (auto [r,c] : changedCells) {
            drawCell(r,c,model);
        }
        window->setDrawable(oldD);
    }
    //restore active viewer
    setViewerId(activeViewer);
}

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

GraphicsDisplay::GraphicsDisplay(int viewerId) : viewerId(viewerId) {
    int windowWidth = BOARD_OFFSET_X * 2 + CELL_SIZE * 8;
    int windowHeight = BOARD_OFFSET_Y * 2 + CELL_SIZE * 8 + INFO_PANEL_HEIGHT * 2;
    window = new Xwindow(windowWidth, windowHeight);
    lastSnapshot.assign(8, vector<char>(8, '.'));
}

GraphicsDisplay::~GraphicsDisplay() {
    delete window;
}

void GraphicsDisplay::notify(GameModel& model, ChangeEvent event) {
    //update view to current player
    setViewerId(model.getCurrentPlayer()->getId());
    switch (event) {
        case ChangeEvent::GameStart:
            //full draw at beginning of game
            drawPlayerInfo(model);
            drawGrid();
            drawBoard(model);
            //capture what is drawn
            lastSnapshot = [&]() {
                vector<vector<char>> snap(8, vector<char>(8, '.'));
                const Board& b = model.getBoard();
                Player* viewer = model.getPlayer(viewerId);
                for (int r = 0; r < 8; r++){
                    for (int c = 0; c < 8; c++) {
                        const Cell& cell = b.at(r,c);
                        char ch = '.';
                        if (cell.getCellType() == CellType::ServerPort) ch = 'S';
                        else if (cell.getLink()) {
                            Link* L = cell.getLink();
                            if (viewer->knowsOpponentLink(L->getId()) || L->isRevealed() || L->getOwner() == viewer) {
                                ch = L->getId();
                            } else {
                                ch = '?';
                            }
                            snap[r][c] = ch;
                        }
                    }
                }
                return snap;
            }();
            window->flush();
            break;
        case ChangeEvent::LinkMoved:
        case ChangeEvent::DownloadOccurred:
            //update only info panel and changed cells
            drawPlayerInfo(model);
                {
                // start with an empty board
                std::vector<std::vector<char>> newSnap(8, std::vector<char>(8, '.'));
                const Board& b = model.getBoard();
                Player* viewer = model.getPlayer(viewerId);

                for (int r = 0; r < 8; ++r) {
                    for (int c = 0; c < 8; ++c) {
                        char ch = '.';
                        const Cell& cell = b.at(r, c);
                        if (cell.getCellType() == CellType::ServerPort) {
                            ch = 'S';
                        } else if (cell.getLink()) {
                            Link* L = cell.getLink();
                            if (viewer->knowsOpponentLink(L->getId())
                            || L->isRevealed() || L->getOwner() == viewer)
                                ch = L->getId();
                            else
                                ch = '?';
                        }

                        // if it differs from what was on‐screen, repaint
                        if (ch != lastSnapshot[r][c]) {
                            drawCell(r, c, model);
                        }
                        newSnap[r][c] = ch;
                    }
                }

                lastSnapshot.swap(newSnap);
            }
            window->flush();
            break;
        case ChangeEvent::AbilityUsed:
        case ChangeEvent::TurnEnded:
            window->clear();
            drawPlayerInfo(model);
            drawGrid();
            drawBoard(model);
            window->flush();
            break;
        case ChangeEvent::GameOver:
            window->drawString(window->getWidth() / 2 - 50, window->getHeight() / 2, 
                             "GAME OVER");
            window->flush();
            break;     
        default:
            break;
    }
}

void GraphicsDisplay::drawPlayerInfo(GameModel& model) {
    Player* viewer = model.getPlayer(viewerId);
    Player* opponent = model.getPlayer(viewerId == 1 ? 2 : 1);
    
    if (!viewer || !opponent) return;
    
    // Draw viewer info at top
    std::stringstream viewerInfo;
    viewerInfo << "Player " << viewerId << " - Downloaded: " 
               << viewer->getDataDownloadCount() << "D, " 
               << viewer->getVirusDownloadCount() << "V - Abilities: " 
               << viewer->getUnusedAbilityCount();
    
    window->drawString(10, 30, viewerInfo.str(), Xwindow::Black);
    
    // Draw viewer's links
    const auto& myLinks = viewer->getLinks();
    int x = 10;
    int y = 50;
    for (const auto& pair : myLinks) {
        char id = pair.first;
        Link* link = pair.second;
        
        std::stringstream linkInfo;
        linkInfo << id << ": ";
        if (link->getType() == LinkType::Virus) {
            linkInfo << "V" << link->getStrength();
            window->drawString(x, y, linkInfo.str(), Xwindow::Red);
        } else {
            linkInfo << "D" << link->getStrength();
            window->drawString(x, y, linkInfo.str(), Xwindow::Green);
        }
        
        x += 80;
        if (x > window->getWidth() - 80) {
            x = 10;
            y += 20;
        }
    }
    
    // Draw opponent info at bottom
    std::stringstream oppInfo;
    oppInfo << "Player " << (viewerId == 1 ? 2 : 1) << " - Downloaded: " 
            << opponent->getDataDownloadCount() << "D, " 
            << opponent->getVirusDownloadCount() << "V - Abilities: " 
            << opponent->getUnusedAbilityCount();
    
    int bottomY = window->getHeight() - 60;
    window->drawString(10, bottomY, oppInfo.str(), Xwindow::Black);
    
    // Draw opponent's links (known information only)
    const auto& oppLinks = opponent->getLinks();
    x = 10;
    y = bottomY + 20;
    for (const auto& pair : oppLinks) {
        char id = pair.first;
        Link* link = pair.second;
        
        std::stringstream linkInfo;
        linkInfo << id << ": ";
        
        if (viewer->knowsOpponentLink(id) || link->isRevealed()) {
            if (link->getType() == LinkType::Virus) {
                linkInfo << "V" << link->getStrength();
                window->drawString(x, y, linkInfo.str(), Xwindow::Red);
            } else {
                linkInfo << "D" << link->getStrength();
                window->drawString(x, y, linkInfo.str(), Xwindow::Green);
            }
        } else {
            linkInfo << "?";
            window->drawString(x, y, linkInfo.str(), Xwindow::Black);
        }
        
        x += 80;
        if (x > window->getWidth() - 80) {
            x = 10;
            y += 20;
        }
    }
}

void GraphicsDisplay::drawGrid() {
    // Draw board grid
    for (int i = 0; i <= 8; ++i) {
        // Vertical lines
        int x = BOARD_OFFSET_X + i * CELL_SIZE;
        window->drawLine(x, BOARD_OFFSET_Y, x, BOARD_OFFSET_Y + 8 * CELL_SIZE, Xwindow::Black);
        
        // Horizontal lines
        int y = BOARD_OFFSET_Y + i * CELL_SIZE;
        window->drawLine(BOARD_OFFSET_X, y, BOARD_OFFSET_X + 8 * CELL_SIZE, y, Xwindow::Black);
    }
}

void GraphicsDisplay::drawCell(int row, int col, GameModel& model) {
    const Board& board = model.getBoard();
    const Cell& cell = board.at(row, col);
    int x = BOARD_OFFSET_X + col * CELL_SIZE;
    int y = BOARD_OFFSET_Y + row * CELL_SIZE;

    //clear cell interior 
    window->fillRectangle(x+1, y+1, CELL_SIZE-2, CELL_SIZE-2, Xwindow::White);
    //redraw grid lines
    window->drawLine(x, y, x + CELL_SIZE, y, Xwindow::Black);
    window->drawLine(x, y, x, y + CELL_SIZE, Xwindow::Black);
    window->drawLine(x + CELL_SIZE, y, x + CELL_SIZE, y + CELL_SIZE, Xwindow::Black);
    window->drawLine(x, y + CELL_SIZE, x + CELL_SIZE, y + CELL_SIZE, Xwindow::Black);

    //draw port or link or firewall just for this cell
    if (cell.getCellType() == CellType::ServerPort) {
        window->fillRectangle(x+5, y+5, CELL_SIZE-10, CELL_SIZE-10, Xwindow::Blue);
        window->drawString(x + CELL_SIZE/2 - 5, y + CELL_SIZE/2 + 5, "S", Xwindow::White);
    }
    else if (cell.getLink()) {
        Link* link = cell.getLink();
        bool isOwner   = link->getOwner()->getId() == viewerId;
        bool isRevealed= link->isRevealed() || isOwner;
        int bg = getLinkColor(link, isRevealed, isOwner);
        window->fillRectangle(x+5, y+5, CELL_SIZE-10, CELL_SIZE-10, bg);
        std::string id(1, link->getId());
        window->drawString(x + CELL_SIZE/2 - 5, y + CELL_SIZE/2 + 5, id, Xwindow::White);
    }
    if (cell.getCellType() == CellType::Firewall) {
        Player* owner = cell.getFirewallOwner();
        if (owner) {
            char f = owner->getId() == 1 ? 'm' : 'w';
            std::string s(1, f);
            window->drawString(x+5, y+CELL_SIZE-15, s, Xwindow::Red);
        }
    }

}

void GraphicsDisplay::drawBoard(GameModel& model) {
    const Board& board = model.getBoard();
    Player* viewer = model.getPlayer(viewerId);
    
    if (!viewer) return;
    
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Cell& cell = board.at(row, col);
            
            int x = BOARD_OFFSET_X + col * CELL_SIZE;
            int y = BOARD_OFFSET_Y + row * CELL_SIZE;
            
            // Fill cell background
            window->fillRectangle(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, Xwindow::White);
            
            if (cell.getCellType() == CellType::ServerPort) {
                // Draw server port
                window->fillRectangle(x + 5, y + 5, CELL_SIZE - 10, CELL_SIZE - 10, Xwindow::Blue);
                window->drawString(x + CELL_SIZE/2 - 5, y + CELL_SIZE/2 + 5, "S", Xwindow::White);
            }
            else if (cell.getLink()) {
                Link* link = cell.getLink();
                bool isOwner = (link->getOwner()->getId() == viewerId);
                bool isRevealed = link->isRevealed() || isOwner;
                
                if (isRevealed) {
                    // Draw colored background based on link type
                    int bgColor = getLinkColor(link, true, isOwner);
                    window->fillRectangle(x + 5, y + 5, CELL_SIZE - 10, CELL_SIZE - 10, bgColor);
                    
                    // Draw link ID
                    std::string linkStr(1, link->getId());
                    window->drawString(x + CELL_SIZE/2 - 5, y + CELL_SIZE/2 + 5, linkStr, Xwindow::White);
                } else {
                    // Unknown opponent link
                    window->fillRectangle(x + 5, y + 5, CELL_SIZE - 10, CELL_SIZE - 10, Xwindow::Black);
                    window->drawString(x + CELL_SIZE/2 - 5, y + CELL_SIZE/2 + 5, "?", Xwindow::White);
                }
            }
            
            // Draw firewall if present
            if (cell.getCellType() == CellType::Firewall) {
                Player* firewallOwner = cell.getFirewallOwner();
                if (firewallOwner) {
                    char firewallChar = (firewallOwner->getId() == 1) ? 'm' : 'w';
                    std::string firewallStr(1, firewallChar);
                    window->drawString(x + 5, y + CELL_SIZE - 15, firewallStr, Xwindow::Red);
                }
            }
        }
    }
}

int GraphicsDisplay::getLinkColor(Link* link, bool isRevealed, bool isOwner) const {
    if (!isRevealed && !isOwner) {
        return Xwindow::Black; // Unknown
    }
    
    if (link->getType() == LinkType::Data) {
        return Xwindow::Green; // Data
    } else {
        return Xwindow::Red; // Virus
    }
}
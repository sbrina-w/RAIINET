#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "board.h"
#include "player.h"
#include "subject.h"
#include "changeevent.h"
#include <memory>

using namespace std; 

class Link;

class GameModel : public Subject
{
private:
    Board board;
    std::vector<std::unique_ptr<Player>> players;
    // continuously incremented, mod by number of players to get cur player
    int currentTurn; 
    vector<pair<int, int>> changedCells;

public:
    GameModel();
    ~GameModel();

    //game setup
    void initializePlayers(const string& p1Abilities, const string& p2Abilities, const vector<string>& p1Links, const vector<string>& p2Links);
    void placeLinksOnBoard();

    //game actions
    void moveLink(Player* player, char id, int dir);
    void useAbility(int abilityID, const vector<string>& args);

    //observer management
    void addObserver(Observer *observer);
    void removeObserver(Observer *observer);
    void notify(ChangeEvent event);

    //game state
    bool isGameOver();
    Player* getPlayer(int playerId) const;
    void nextTurn();
    bool isLinkOnBoard(Link* link) const;
    void addChangedCell(int row, int col);
    void clearChangedCells();
    int getWinnerId() const;

    //accessors
    Board &getBoard();
    const Board &getBoard() const;
    int getCurrentTurn() const;
    Player* getCurrentPlayer() const;
    Link* findLinkById(char linkId) const;
    const vector<pair<int, int>>& getChangedCells() const;
};

#endif

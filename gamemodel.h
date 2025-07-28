#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "board.h"
#include "player.h"
#include "subject.h"
#include "changeevent.h"

using namespace std; 

class Link;

class GameModel : public Subject
{
private:
    Board board;
    std::vector<Player *> players;
    // continuously incremented, mod by number of players to get cur player
    int currentTurn; 
    int lastOldR, lastOldC, lastNewR, lastNewC;

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

    //accessors
    Board &getBoard();
    const Board &getBoard() const;
    int getCurrentTurn() const;
    Player* getCurrentPlayer() const;
    Link* findLinkById(char linkId) const;
    int getLastOldR() const;
    int getLastOldC() const;
    int getLastNewR() const;
    int getLastNewC() const;
};

#endif

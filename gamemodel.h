#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <vector>
#include "board.h"
#include "player.h"
#include "subject.h"
#include "changeevent.h"

using namespace std; 

class GameModel : public Subject
{
private:
    Board board;
    std::vector<Player *> players;
    // continuously incremented, mod by number of players to get cur player
    int currentTurn; 

public:
    GameModel();
    ~GameModel();

    //game setup
    void initializePlayers(const string& p1Abilities, const string& p2Abilities, const vector<string>& p1Links, const vector<string>& p2Links);
    void placeLinksOnBoard();

    //game actions
    void moveLink(char id, int dir);
    void useAbility(int playerID, int abilityID, int target);

    //observer management
    void addObserver(Observer *observer);
    void removeObserver(Observer *observer);
    void notify(ChangeEvent event);

    //game state
    bool isGameOver();
    Player* getPlayer(int playerId) const;
    void nextTurn();

    //accessors
    Board &getBoard();
    const Board &getBoard() const;
    int getCurrentTurn() const;
    Player* getCurrentPlayer() const;
};

#endif

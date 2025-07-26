#include "gamemodel.h"
#include "link.h"
#include <iostream>

using namespace std;

GameModel::GameModel()
    : board(),
      currentTurn(1)
{
    // create two players with IDs 1 and 2
    players.push_back(new Player(1));
    players.push_back(new Player(2));
}

GameModel::~GameModel()
{
    for (Player *p : players)
        delete p;
}

void GameModel::initializePlayers(const string &p1Abilities, const string &p2Abilities,
                                  const vector<string> &p1Links, const vector<string> &p2Links)
{
    //set up abilities
    players[0]->setAbilities(p1Abilities);
    players[1]->setAbilities(p2Abilities);

    //create and assign links for player 1 (a-h)
    char linkId = 'a';
    for (const std::string& linkStr : p1Links) {
        LinkType type = (linkStr[0] == 'V') ? LinkType::Virus : LinkType::Data;
        int strength = linkStr[1] - '0'; //convert char to int
        Link* link = new Link(linkId, type, strength, players[0]);
        players[0]->addLink(linkId, link);
        linkId++;
    }
    
    //create and assign links for player 2 (A-H)
    linkId = 'A';
    for (const std::string& linkStr : p2Links) {
        LinkType type = (linkStr[0] == 'V') ? LinkType::Virus : LinkType::Data;
        int strength = linkStr[1] - '0';
        Link* link = new Link(linkId, type, strength, players[1]);
        players[1]->addLink(linkId, link);
        linkId++;
    }
}

void GameModel::placeLinksOnBoard() {
    //place player 1's links on rows 0 and 1
    const auto& p1Links = players[0]->getLinks();
    int col = 0;
    for (const auto& pair : p1Links) {
        char id = pair.first;
        Link* link = pair.second;
        
        if (col == 3 || col == 4) {
            //skip server ports, place on row 1 instead
            board.at(1, col).setLink(link);
        } else {
            //place on row 0
            board.at(0, col).setLink(link);
        }
        col++;
    }
    
    //place player 2's links on rows 6 and 7
    const auto& p2Links = players[1]->getLinks();
    col = 0;
    for (const auto& pair : p2Links) {
        char id = pair.first;
        Link* link = pair.second;
        
        if (col == 3 || col == 4) {
            //skip server ports, place on row 6 instead
            board.at(6, col).setLink(link);
        } else {
            //place on row 7
            board.at(7, col).setLink(link);
        }
        col++;
    }
}

bool GameModel::isGameOver()
{
    for (Player* player : players) {
        if (player->getDataDownloadCount() >= 4 || player->getVirusDownloadCount() >= 4) {
            return true;
        }
    }
    return false;
}

void GameModel::moveLink(char /*id*/, int /*dir*/)
{
    // TODO: implement movement
    // …
    notifyObservers(ChangeEvent::LinkMoved);
}

Player* GameModel::getPlayer(int playerId) const {
    for (Player* player : players) {
        if (player->getId() == playerId) {
            return player;
        }
    }
    return nullptr;
}

Player* GameModel::getCurrentPlayer() const {
    return getPlayer(currentTurn);
}

void GameModel::useAbility(int /*playerID*/, int /*abilityID*/, int /*target*/)
{
    // TODO: implement abilities
    // …
    notifyObservers(ChangeEvent::AbilityUsed);
}

// Observer management
void GameModel::addObserver(Observer *o)
{
    attach(o);
}
void GameModel::removeObserver(Observer *o)
{
    detach(o);
}

// Notify wrapper
void GameModel::notify(ChangeEvent e)
{
    notifyObservers(e);
}

// Getters for views
Board &GameModel::getBoard()
{
    return board;
}
const Board &GameModel::getBoard() const
{
    return board;
}
int GameModel::getCurrentTurn() const
{
    return currentTurn;
}

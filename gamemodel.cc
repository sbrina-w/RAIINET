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
    // set up abilities
    players[0]->setAbilities(p1Abilities);
    players[1]->setAbilities(p2Abilities);

    // create and assign links for player 1 (a-h)
    char linkId = 'a';
    for (const std::string &linkStr : p1Links)
    {
        LinkType type = (linkStr[0] == 'V') ? LinkType::Virus : LinkType::Data;
        int strength = linkStr[1] - '0'; // convert char to int
        Link *link = new Link(linkId, type, strength, players[0]);
        players[0]->addLink(linkId, link);
        linkId++;
    }

    // create and assign links for player 2 (A-H)
    linkId = 'A';
    for (const std::string &linkStr : p2Links)
    {
        LinkType type = (linkStr[0] == 'V') ? LinkType::Virus : LinkType::Data;
        int strength = linkStr[1] - '0';
        Link *link = new Link(linkId, type, strength, players[1]);
        players[1]->addLink(linkId, link);
        linkId++;
    }
}

void GameModel::placeLinksOnBoard()
{
    // place player 1's links on rows 0 and 1
    const auto &p1Links = players[0]->getLinks();
    int col = 0;
    for (const auto &pair : p1Links)
    {
        char id = pair.first;
        Link *link = pair.second;

        if (col == 3 || col == 4)
        {
            // skip server ports, place on row 1 instead
            board.at(1, col).setLink(link);
        }
        else
        {
            // place on row 0
            board.at(0, col).setLink(link);
        }
        col++;
    }

    // place player 2's links on rows 6 and 7
    const auto &p2Links = players[1]->getLinks();
    col = 0;
    for (const auto &pair : p2Links)
    {
        char id = pair.first;
        Link *link = pair.second;

        if (col == 3 || col == 4)
        {
            // skip server ports, place on row 6 instead
            board.at(6, col).setLink(link);
        }
        else
        {
            // place on row 7
            board.at(7, col).setLink(link);
        }
        col++;
    }
}

bool GameModel::isGameOver()
{
    for (Player *player : players)
    {
        if (player->getDataDownloadCount() >= 4 || player->getVirusDownloadCount() >= 4)
        {
            return true;
        }
    }
    return false;
}

void GameModel::moveLink(char id, int dir)
{
    Player *curr = getCurrentPlayer();
    Link *link = curr->getLink(id);

    if (!link)
    {
        throw std::invalid_argument("No such link for current player");
    }

    // finding the right link
    int oldR = -1, oldC = -1;
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (board.at(r, c).getLink() == link)
            {
                oldR = r;
                oldC = c;
            }
        }
    }
    if (oldR < 0)
    {
        throw std::runtime_error("Link not on board");
    }

    // find the new row and column
    static constexpr int dr[4] = {-1, 1, 0, 0};
    static constexpr int dc[4] = {0, 0, -1, 1};
    int steps = link->isBoosted() ? 2 : 1;
    int newR = oldR + dr[dir] * steps;
    int newC = oldC + dc[dir] * steps;

    // now check that the new row and column are allowable

    // within x bounds
    if (newC < 0 || newC > 7)
    {
        throw std::invalid_argument("Cannot move off left/right edge");
    }

    // within the y bounds:
    if (newR < 0 || newR > 7)
    {
        bool allowed = (newR < 0 && curr->getId() == 2) || (newR > 7 && curr->getId() == 1);
        if (!allowed)
        {
            throw std::invalid_argument("Cannot move off that edge");
        }
        // TODO: perform download of your own link
        // curr->incrementDownload(link->getType());
        board.at(oldR, oldC).removeLink();
        notifyObservers(ChangeEvent::DownloadOccurred);
        return;
    }

    // in bounds so check the cell currently there:
    Cell &dest = board.at(newR, newC);

    // for if destination is a server port
    if (dest.getCellType() == CellType::ServerPort)
    {
        bool isMyPort = (curr->getId() == 1 && newR == 7) || (curr->getId() == 2 && newR == 0);
        if (isMyPort)
        {
            throw std::invalid_argument("Cannot move onto your own server port");
        }
        // otherwise it *is* the opponent’s port → they download your link
        Player *opp = getPlayer(curr->getId() == 1 ? 2 : 1);
        // TODO: opp->incrementDownload(link->getType());
        board.at(oldR, oldC).removeLink();
        notifyObservers(ChangeEvent::DownloadOccurred);
        return;
    }

    // if own link:
    if (dest.getLink() && dest.getLink()->getOwner() == curr)
    {
        throw std::invalid_argument("Cannot move onto your own link");
    }

    // 8) If there’s an opponent link we battle
    if (dest.getLink())
    {
        Link *other = dest.getLink();
        link->reveal();
        other->reveal();
        if (link->getStrength() >= other->getStrength())
        {
            // current player wins so the opponent downloads
            Player *opp = other->getOwner();
            // opp->incrementDownload(other->getType());
            dest.removeLink();
            dest.setLink(link);
            board.at(oldR, oldC).removeLink();
            notifyObservers(ChangeEvent::DownloadOccurred);
        }
        else
        {
            // you lose ⇒ so you download
            // TODO: curr->incrementDownload(link->getType());
            board.at(oldR, oldC).removeLink();
            notifyObservers(ChangeEvent::DownloadOccurred);
        }
    }
    else
    {
        // regular move
        dest.setLink(link);
        board.at(oldR, oldC).removeLink();
        notifyObservers(ChangeEvent::LinkMoved);
    }
}

Player *GameModel::getPlayer(int playerId) const
{
    for (Player *player : players)
    {
        if (player->getId() == playerId)
        {
            return player;
        }
    }
    return nullptr;
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

Player *GameModel::getCurrentPlayer() const
{
    return players[(currentTurn - 1) % players.size()];
}

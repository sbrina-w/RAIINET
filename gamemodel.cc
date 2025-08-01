#include "gamemodel.h"
#include "link.h"
#include <iostream>
#include <memory>

using namespace std;

GameModel::GameModel()
    : board(),
      currentTurn(1)
{
    // create two players with IDs 1 and 2
    players.push_back(std::make_unique<Player>(1));
    players.push_back(std::make_unique<Player>(2));
}

GameModel::~GameModel(){
    
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
        auto link = std::make_unique<Link>(linkId, type, strength, players[0].get());
        players[0]->addLink(linkId, std::move(link));
        linkId++;
    }

    // create and assign links for player 2 (A-H)
    linkId = 'A';
    for (const std::string &linkStr : p2Links)
    {
        LinkType type = (linkStr[0] == 'V') ? LinkType::Virus : LinkType::Data;
        int strength = linkStr[1] - '0';
        auto link = std::make_unique<Link>(linkId, type, strength, players[1].get());
        players[1]->addLink(linkId, std::move(link));
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
        Link *link = pair.second.get();

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
        Link *link = pair.second.get();

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
    for (const auto &player : players)
    {
        if (player->getDataDownloadCount() >= 4 || player->getVirusDownloadCount() >= 4)
        {
            return true;
        }
    }
    return false;
}

void GameModel::nextTurn() {
    ++currentTurn;
    notifyObservers(ChangeEvent::TurnEnded);
    getCurrentPlayer()->startTurn();
}

bool GameModel::isLinkOnBoard(Link* link) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board.at(r, c).getLink() == link) {
                return true;
            }
        }
    }
    return false;
}

void GameModel::moveLink(Player* curr, char id, int dir)
{
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

    // check if outside x bounds
    if (newC < 0 || newC > 7)
    {
        throw std::invalid_argument("Cannot move off left/right edge");
    }

    // check if outside y bounds (either not allowed or downloading to be done)
    if (newR < 0 || newR > 7)
    {
        bool allowed = (newR < 0 && curr->getId() == 2) || (newR > 7 && curr->getId() == 1);
        if (!allowed)
        {
            throw std::invalid_argument("Cannot move off that edge");
        }

        Player *opp = getPlayer(curr->getId() == 1 ? 2 : 1);

        // downloading your own link after moving it off the edge, reveal it to oppo for game consistency
        link->reveal();
        opp->learnOpponentLink(link->getId(), link); // opponent gets to learn it
        curr->incrementDownload(link->getType());
        board.at(oldR, oldC).removeLink();
        //mark old cell as changed (link downloaded/removed from display)
        clearChangedCells();
        addChangedCell(oldR, oldC);
        notifyObservers(ChangeEvent::DownloadOccurred);
        return;
    }

    // in bounds so check the cell currently there:
    Cell &dest = board.at(newR, newC);

    // for if destination is a firewall cell
    if (dest.getCellType() == CellType::Firewall) {
        Player* firewallOwner = dest.getFirewallOwner();
        if (firewallOwner && firewallOwner != curr) {
            // only affect opponent links
            link->reveal();
            firewallOwner->learnOpponentLink(link->getId(), link);

            if (link->getType() == LinkType::Virus) {
                // virus is downloaded by its owner (curr)
                curr->incrementDownload(link->getType());
                board.at(oldR, oldC).removeLink();
                clearChangedCells();
                addChangedCell(oldR, oldC);
                notifyObservers(ChangeEvent::DownloadOccurred);
                return;
            }
            // if not a virus, continue with normal movement (could be battle, etc.)
        }
    }

    // for if destination is a server port
    if (dest.getCellType() == CellType::ServerPort)
    {
        bool isMyPort = (curr->getId() == 1 && newR == 0) || (curr->getId() == 2 && newR == 7);
        if (isMyPort)
        {
            throw std::invalid_argument("Cannot move onto your own server port");
        }
        // otherwise it *is* the opponent’s port → they download your link
        // need to reveal to them what the link is too
        Player *opp = getPlayer(curr->getId() == 1 ? 2 : 1);

        link->reveal(); //makes the link revealed
        opp->learnOpponentLink(link->getId(), link); //opponent gets to learn it
        opp->incrementDownload(link->getType()); //then they increment their download count
        board.at(oldR, oldC).removeLink();
        //mark old cell as changed
        clearChangedCells();
        addChangedCell(oldR, oldC);
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

        //reveals both links
        link->reveal();
        other->reveal();

        //both learn about the other's link
        //we learn about theirs
        curr->learnOpponentLink(other->getId(), other);

        //they learn about us
        Player *opp = other->getOwner();
        opp->learnOpponentLink(link->getId(), link);

        if (link->getStrength() >= other->getStrength())
        {
            // current player wins, winner downloads loser's link
            curr->incrementDownload(other->getType());
            dest.removeLink();
            dest.setLink(link);
            board.at(oldR, oldC).removeLink();
            //mark both cells as changed (old position cleared, new position has winning link)
            clearChangedCells();
            addChangedCell(oldR, oldC);
            addChangedCell(newR, newC);
            notifyObservers(ChangeEvent::DownloadOccurred);
        }
        else
        {
            // you lose ⇒ so opponent (winner) downloads your link
            opp->incrementDownload(link->getType());
            board.at(oldR, oldC).removeLink();
            clearChangedCells();
            addChangedCell(oldR, oldC);
            addChangedCell(newR, newC);
            notifyObservers(ChangeEvent::DownloadOccurred);
        }
    }
    else
    {
        // regular move
        dest.setLink(link);
        board.at(oldR, oldC).removeLink();
        clearChangedCells();
        addChangedCell(oldR, oldC);
        addChangedCell(newR, newC);
        notifyObservers(ChangeEvent::LinkMoved);
    }
}

Player *GameModel::getPlayer(int playerId) const
{
    for (const auto &player : players)
    {
        if (player->getId() == playerId)
        {
            return player.get();
        }
    }
    return nullptr;
}

void GameModel::useAbility(int abilityID, const std::vector<std::string>& args) {
    Player* player = getCurrentPlayer();
    if (!player) throw std::runtime_error("useAbility: getCurrentPlayer failed");

    if (!player->canUseAbility()) {
        throw std::runtime_error("You have already used all allowed abilities this turn.");
    }

    const auto& abilities = player->getAbilities();
    if (abilityID < 1 || abilityID > static_cast<int>(abilities.size()))
        throw std::invalid_argument("useAbility: Invalid ability index, must be between 1-5.");

    const auto& ability = abilities.at(abilityID - 1);

    if (ability->isUsed()) throw runtime_error("The ability in this slot has already been used.");

    ability->execute(*this, args);

    player->incrementAbilitiesUsed();
    player->decrementAbilitiesRemaining();

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
    return players[(currentTurn - 1) % players.size()].get();
}

// helper for link related abilities
Link* GameModel::findLinkById(char linkId) const {
    for (const auto &player : players) {
        Link* link = player->getLink(linkId);
        if (link) return link;
    }
    return nullptr;
}

void GameModel::addChangedCell(int row, int col) {
    changedCells.push_back({row, col});
}

void GameModel::clearChangedCells() {
    changedCells.clear();
}

const std::vector<std::pair<int, int>>& GameModel::getChangedCells() const {
    return changedCells;
}

int GameModel::getWinnerId() const {
    // 1. check if any player has 4+ data downloads
    for (const auto & player : players) {
        if (player->getDataDownloadCount() >= 4) {
            return player->getId();
        }
    }

    // 2. check if any player has all opponents with 4+ virus downloads
    for (const auto & candidate : players) {
        bool allOpponentsHave4Virus = true;
        for (const auto & opponent : players) {
            if (opponent == candidate) continue;
            if (opponent->getVirusDownloadCount() < 4) {
                allOpponentsHave4Virus = false;
                break;
            }
        }
        if (allOpponentsHave4Virus) {
            return candidate->getId();
        }
    }

    // no winner yet
    return 0;
}

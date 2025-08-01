#ifndef ABILITY_H
#define ABILITY_H

#include <vector>
#include <string>

class GameModel;

class Ability {
private:
    bool used;
    char abilityID;

protected:
    void markUsed();

public:
    Ability(char id);
    virtual ~Ability() = default;
    virtual void execute(GameModel& model, std::vector<std::string> args) = 0;
    bool isUsed() const;
    char getID() const;
};

#endif

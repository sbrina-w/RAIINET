#ifndef OBSERVER_H
#define OBSERVER_H

#include "changeevent.h"

class GameModel;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void notify(GameModel& model, ChangeEvent event) = 0;
};

#endif

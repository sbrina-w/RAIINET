#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "changeevent.h"

class Observer;

class Subject {
private:
    std::vector<Observer*> observers;

public:
    virtual ~Subject() = default;
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notifyObservers(ChangeEvent event);
};

#endif

#include "subject.h"
#include "gamemodel.h"
#include "observer.h"
#include <algorithm>

void Subject::attach(Observer* observer) {
    observers.push_back(observer);
}

void Subject::detach(Observer* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end());
}

void Subject::notifyObservers(ChangeEvent event) {
    for (auto* obs : observers) {
        // We know Subject is a base of GameModel,
        // so we static_cast back in order to call notify(...)
        obs->notify(static_cast<GameModel&>(*this), event);
    }
}

#include "gamemodel.h"
#include "textdisplay.h"
#include "changeevent.h"

#include <iostream>

int main() {
    GameModel model;
    model.getBoard().initServerPorts();

    TextDisplay view1(1), view2(2);      // ← pass in 1 and 2!
    model.addObserver(&view1);
    model.addObserver(&view2);

    model.notify(ChangeEvent::TurnEnded);
    std::cout << "\n-- Simulating a dummy move --\n";
    model.moveLink('a', 0);

    return 0;
}
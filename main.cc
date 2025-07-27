#include "gamemodel.h"
#include "textdisplay.h"
#include "changeevent.h"
#include "gamesetup.h"
#include "gamecontroller.h"
#include "graphicsdisplay.h"

#include <iostream>

int main(int argc, char *argv[])
{
    // parse command line arguments
    GameSetup setup;
    // could include error message or usage of commands,
    if (!setup.parseCommandLine(argc, argv))
    {
        return 1;
    }

    const GameSetupConfig &config = setup.getConfig();
    // initialize game model
    GameModel model;

    // initialize the server ports
    model.getBoard().initServerPorts();

    // set up displays based on config
    TextDisplay view1(1), view2(2); // ← pass in 1 and 2!
    model.addObserver(&view1);
    model.addObserver(&view2);

    GraphicsDisplay* graphicsView = nullptr;
    if (config.graphicsEnabled) {
        graphicsView = new GraphicsDisplay();
        model.addObserver(graphicsView);
    }

    // put the model in setup, if it fails exit
    if (!setup.initializeGame(model))
    {
        std::cerr << "Failed to initialize game with provided config.\n";
        if (config.graphicsEnabled && graphicsView) delete graphicsView;
        return 1;
    }

    // print statements to debug ability setup
    for (int i = 1; i <= 2; ++i) {
        Player* player = model.getPlayer(i);
        if (!player) continue;
        std::cout << "Player " << i << " abilities:\n";
        // You may need to add a getter for the abilities vector if it's private
        // For now, add this method to Player:
        // const std::vector<Ability*>& getAbilities() const { return abilities; }
        for (const auto& ability : player->getAbilities()) {
            std::cout << "  ID: " << ability->getID()
                      << " Used: " << (ability->isUsed() ? "yes" : "no") << "\n";
        }
    }

    // to draw the initial board
    model.notify(ChangeEvent::GameStart);


    

    // 5) Enter game loop
    GameController controller(model);
    controller.play();

    if (graphicsView) {
        delete graphicsView;
    }
    return 0;
}

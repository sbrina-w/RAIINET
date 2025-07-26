#include "gamemodel.h"
#include "textdisplay.h"
#include "changeevent.h"
#include "gamesetup.h"
#include "gamecontroller.h"
// #include "graphicsdisplay.h" //uncomment later

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

    // uncomment when implementing graphics display

    // GraphicsDisplay* graphicsView = nullptr;
    // if (config.graphicsEnabled) {
    //     graphicsView = new GraphicsDisplay();
    //     model.addObserver(graphicsView);
    // }

    // put the model in setup, if it fails exit
    if (!setup.initializeGame(model))
    {
        std::cerr << "Failed to initialize game with provided config.\n";
        // if (graphicsView) delete graphicsView;
        // return 1;
    }

    // to draw the initial board
    model.notify(ChangeEvent::GameStart);

    // 5) Enter game loop
    GameController controller(model);
    controller.play();

    // for later
    //  if (graphicsView) {
    //      delete graphicsView;
    //  }
    return 0;
}
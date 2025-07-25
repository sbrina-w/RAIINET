#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H

#include "observer.h"
#include "xwindow.h"

class GraphicsDisplay : public Observer {
private:
    XWindow* window;

public:
    GraphicsDisplay();
    ~GraphicsDisplay();
    void notify(GameModel& model, ChangeEvent event) override;
};

#endif

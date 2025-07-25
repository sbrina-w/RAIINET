#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"

class TextDisplay : public Observer {
public:
    void notify(GameModel& model, ChangeEvent event) override;
};

#endif

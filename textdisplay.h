#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"

class TextDisplay : public Observer
{
    int viewerId;

public:
    explicit TextDisplay(int viewerId): viewerId(viewerId) {}
    void notify(GameModel &model, ChangeEvent event) override;
};

#endif

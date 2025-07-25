#ifndef XWINDOW_H
#define XWINDOW_H

class XWindow {
public:
    void fillRectangle(int x, int y, int width, int height);
    void drawString(int x, int y, const char* text);
};

#endif

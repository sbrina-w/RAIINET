#ifndef XWINDOW_H
#define XWINDOW_H
#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window   win;       // the on-screen window
  Drawable drawable;  // current drawing target
  GC       gc;
  int      width, height;
  unsigned long colours[5];

public:
  enum { White=0, Black, Red, Green, Blue };

  Xwindow(int width=500, int height=500);
  ~Xwindow();

  int getWidth()  const { return width; }
  int getHeight() const { return height; }

  //access the Display* for XFreePixmap / XCopyArea
  Display* getDisplay() const { return d; }

  void fillRectangle(int x,int y,int w,int h,int colour=Black);
  void drawString   (int x,int y,const std::string &s,int colour=Black);
  void drawLine     (int x1,int y1,int x2,int y2,int colour=Black);
  void clear        ();      // clears only the on-screen window
  void flush        ();      // flush to screen

  //off-screen pixmap helpers
  Pixmap makePixmap() const {
    return XCreatePixmap(d, win, width, height,
                         DefaultDepth(d, DefaultScreen(d)));
  }

  void copyPixmap(Pixmap src,
                  int sx,int sy,
                  unsigned int w,unsigned int h,
                  int dx,int dy)
  {
    XCopyArea(d, src, win, gc, sx, sy, w, h, dx, dy);
    flush();
  }

  //change the drawing target (win or a Pixmap)
  Drawable getDrawable() const { return drawable; }
  void     setDrawable(Drawable dr) { drawable = dr; }
};

#endif

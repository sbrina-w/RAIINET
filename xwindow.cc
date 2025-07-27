#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "xwindow.h"

using namespace std;

Xwindow::Xwindow(int width, int height) : width{width}, height{height} {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  int scr = DefaultScreen(d);
  win = XCreateSimpleWindow(d,
                            RootWindow(d, scr),
                            10,10,
                            width, height,
                            1,
                            BlackPixel(d, scr),
                            WhitePixel(d, scr));
  drawable = win;
  XSelectInput(d, win, ExposureMask | KeyPressMask);
  XMapRaised(d, win);

  gc = XCreateGC(d, win, 0, nullptr);
  Colormap cmap = DefaultColormap(d, scr);
  const char *names[5] = {"white","black","red","green","blue"};
  for (int i = 0; i < 5; ++i) {
    XColor col;
    XParseColor(d, cmap, names[i], &col);
    XAllocColor(d, cmap, &col);
    colours[i] = col.pixel;
  }
  XSetForeground(d, gc, colours[Black]);
  XSynchronize(d, false);
  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x,int y,int w,int h,int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, drawable, gc, x,y,w,h);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x,int y,const string &s,int colour) {
  XSetForeground(d, gc, colours[colour]);
  XDrawString(d, drawable, gc, x, y, s.c_str(), s.size());
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawLine(int x1,int y1,int x2,int y2,int colour) {
  XSetForeground(d, gc, colours[colour]);
  XDrawLine(d, drawable, gc, x1,y1,x2,y2);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::clear() {
  XClearWindow(d, win);
}

void Xwindow::flush() {
  XFlush(d);
}

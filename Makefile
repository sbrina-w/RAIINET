CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# List all your source files here
SRCS = main.cc gamemodel.cc gamesetup.cc gamecontroller.cc board.cc cell.cc link.cc player.cc subject.cc textdisplay.cc \
ability.cc linkboost.cc firewall.cc download.cc scan.cc polarize.cc exchange.cc golater.cc hijack.cc

OBJS = $(SRCS:.cc=.o)

# Graphics display source
GRAPHICS_SRCS = graphicsdisplay.cc xwindow.cc
GRAPHICS_OBJS = $(GRAPHICS_SRCS:.cc=.o)

# If you want to build the graphics-demo separately
GRAPHICS_DEMO_SRCS = graphics-demo/graphicsdemo.cc graphics-demo/window.cc
GRAPHICS_DEMO_OBJS = $(GRAPHICS_DEMO_SRCS:.cc=.o)

GRAPHICS_LIBS = -lX11
GRAPHICS_CXXFLAGS = -DHAVE_X11
ALL_OBJS = $(OBJS) $(GRAPHICS_OBJS)

# The main executable
raiinet: $(ALL_OBJS)
	$(CXX) $(CXXFLAGS) $(GRAPHICS__CXXFLAGS) -o $@ $^ $(GRAPHICS_LIBS)

# Graphics demo executable
graphicsdemo: $(GRAPHICS_DEMO_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lX11

# Pattern rule for compiling .cc to .o
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
.PHONY: clean
clean:
	rm -f $(OBJS) $(GRAPHICS_OBJS) $(GRAPHICS_DEMO_OBJS) raiinet graphicsdemo

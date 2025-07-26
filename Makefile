CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# List all your source files here
SRCS = main.cc gamemodel.cc gamesetup.cc gamecontroller.cc board.cc cell.cc link.cc player.cc subject.cc textdisplay.cc
OBJS = $(SRCS:.cc=.o)

# If you want to build the graphics-demo separately
GRAPHICS_DEMO_SRCS = graphics-demo/graphicsdemo.cc graphics-demo/window.cc
GRAPHICS_DEMO_OBJS = $(GRAPHICS_DEMO_SRCS:.cc=.o)

# The main executable
raiinet: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Graphics demo executable
graphicsdemo: $(GRAPHICS_DEMO_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lX11

# Pattern rule for compiling .cc to .o
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
.PHONY: clean
clean:
	rm -f $(OBJS) $(GRAPHICS_DEMO_OBJS) raiinet graphicsdemo

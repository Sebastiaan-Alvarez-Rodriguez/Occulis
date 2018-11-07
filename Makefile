#
# Makefile for final project Computer Graphics
# Sebastiaan Alvarez Rodriguez
#

CXX       = g++ 
WARNINGS  = -Wall -Wextra -pedantic
OTHERS    = -g `sdl-config --cflags`

LFLAGS    =
YFLAGS    = --defines=gen/y.tab.h --debug --verbose
LDFLAGS   = -g
LOADLIBS  = `sdl-config --libs` -lGL -lGLU -lGLEW

SRCDIR    = src/
SRCFILES  = main.cpp Self.cpp
SRCS      = $(addprefix $(SRCDIR),$(SRCFILES))

OBJDIR    = obj/
OBJFILES  = $(SRCFILES:.cpp=.o)
OBJS      = $(addprefix $(OBJDIR),$(OBJFILES))

IDIRS     = -I$(SRCDIR)
CXXFLAGS  = -std=c++11 $(WARNINGS) $(OTHERS) $(IDIRS)

EXEC      = Self

.PHONY: all first clean dirs

all: $(EXEC)

# To be executed before the very first build
first: dirs

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LOADLIBS)

clean:
	rm -f $(OBJDIR){*.o}* $(EXEC)

dirs:
	mkdir $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

c: clean

.PHONY: c

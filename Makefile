#
# Makefile for final project Computer Graphics
# Sebastiaan Alvarez Rodriguez
#

CXX           ?= g++ 
WARNINGS       = -Wall -Wextra -pedantic -O3
OTHERS         = `sdl-config --cflags`


SRCDIR         = src
SRCFILES       = $(notdir $(shell find $(SRCDIR)/*.cpp))
SRCS           = $(addprefix $(SRCDIR),$(SRCFILES))

OBJDIR         = obj/
OBJFILES       = $(SRCFILES:.cpp=.o)
OBJS           = $(addprefix $(OBJDIR),$(OBJFILES))

IDIRS          = -I$(SRCDIR) $(I_WARNING_DIRS)
I_WARNING_DIRS = -isystem lib

CXXFLAGS       = $(IDIRS) -std=c++11 $(WARNINGS) $(OTHERS)
CFLAGS         = $(IDIRS) `sdl-config --libs` -lGL -lGLU -lGLEW
EXEC           = Occulis

MAKEFLAGS      = -j

.PHONY: all run build clean

all: build

run: build
	./$(EXEC)
build: $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(CFLAGS)

clean:
	rm -f $(OBJDIR)*.o $(EXEC)

$(OBJDIR)%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

c: clean

.PHONY: c

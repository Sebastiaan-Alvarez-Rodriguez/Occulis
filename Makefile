#
# Makefile for final project Computer Graphics
# Sebastiaan Alvarez Rodriguez
#

CXX       = g++ 
WARNINGS  = -Wall -Wextra -pedantic
OTHERS    = `sdl-config --cflags`


SRCDIR    = src
SRCFILES  = main.cpp Self.cpp
SRCS      = $(addprefix $(SRCDIR),$(SRCFILES))

OBJDIR    = obj
OBJFILES  = $(SRCFILES:.cpp=.o)
OBJS      = $(addprefix $(OBJDIR),$(OBJFILES))

IDIRS     = -I$(SRCDIR)

CXXFLAGS  = -std=c++11 $(WARNINGS) $(OTHERS)
CFLAGS    = `sdl-config --libs` -lGL -lGLU -lGLEW
EXEC      = Self

.PHONY: all build first clean dirs

all: build

# To be executed before the very first build
first: dirs

build: $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(CFLAGS)

clean:
	rm -f $(OBJDIR)*.o $(EXEC)

dirs:
	mkdir $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

c: clean

.PHONY: c

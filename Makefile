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

OBJDIR    = obj/
OBJFILES  = $(SRCFILES:.cpp=.o)
OBJS      = $(addprefix $(OBJDIR),$(OBJFILES))

IDIRS     = -I$(SRCDIR)

CXXFLAGS  = -std=c++11 $(WARNINGS) $(OTHERS)
CFLAGS    = `sdl-config --libs` -lGL -lGLU -lGLEW
EXEC      = Self

MAKEFLAGS = -j

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

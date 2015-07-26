OBJS=main.o linuxmain.o field.o popper.o textures.o
CXX=g++
CXXFLAGS=-g -std=c++11
LIBS=-lX11 -lGL -lGLU -lglut

all: chained

clean:
	$(RM) $(OBJS) chained

chained: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

field.o: field.cpp field.h swapstack.h game.h block.h textures.h
linuxmain.o: linuxmain.cpp root.h vkeys.h textures.h
main.o: main.cpp winmain.h constants.h vkeys.h scoringsystem.h tga.h block.h clearingblock.h field.h game.h garbage.h popper.h swap.h swapstack.h root.h textures.h
popper.o: popper.cpp popper.h field.h
scoringsystem.o: scoringsystem.cpp scoringsystem.h
winmain.o: winmain.cpp winmain.h root.h
sample.o: sample.cpp

sample: sample.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

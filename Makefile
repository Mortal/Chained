OBJS=main.o linuxmain.o field.o popper.o
CXX=g++
CXXFLAGS=-g -lX11 -lGL

all: chained

clean:
	$(RM) $(OBJS) chained

chained: $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $@

field.o: field.cpp field.h swapstack.h game.h
linuxmain.o: linuxmain.cpp root.h vkeys.h
main.o: main.cpp winmain.h constants.h vkeys.h scoringsystem.h tga.h block.h clearingblock.h field.h game.h garbage.h popper.h swap.h swapstack.h root.h
popper.o: popper.cpp popper.h field.h
scoringsystem.o: scoringsystem.cpp scoringsystem.h
winmain.o: winmain.cpp winmain.h root.h

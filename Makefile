OBJS=main.o linuxmain.o field.o popper.o

all: chained

CXX=g++
CXXFLAGS=-g -lX11 -lGL

chained: $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $@

main.o: main.cpp block.h clearingblock.h constants.h field.h game.h garbage.h img.questionmark.h popper.h root.h scoringsystem.h swap.h tga.h vkeys.h
linuxmain.o: linuxmain.cpp root.h vkeys.h

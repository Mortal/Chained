OBJS=main.o linuxmain.o

all: chained

chained: $(OBJS)
	g++ -g -lX11 -lGL $(OBJS) -o $@

.cpp.o:
	g++ -g -c $< -o $@

main.o: main.cpp block.h clearingblock.h constants.h field.h game.h garbage.h img.questionmark.h popper.h root.h scoringsystem.h swap.h tga.h vkeys.h
linuxmain.o: linuxmain.cpp root.h vkeys.h

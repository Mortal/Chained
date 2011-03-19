OBJS=main.o linuxmain.o

all: chained

chained: $(OBJS)
	g++ -g -lX11 -lGL -lGLU $(OBJS) -o $@

.cpp.o:
	g++ -g -c $< -o $@

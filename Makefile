OBJS=main.o linuxmain.o

all: chained

chained: $(OBJS)
	g++ -lX11 -lGL -lGLU $(OBJS) -o $@

.cpp.o:
	g++ -c $< -o $@

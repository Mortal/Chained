/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_ROOT
#define CHAINED_ROOT
#include "constants.h"
#include <fstream>

extern int substate;
extern int state;
extern int CLOCKS_PER_FRAME;
extern class Game *game[8];
extern int numply;
extern int newnumply;
void Key(int key, byte dir);
void KeyDown(int key);
void KeyUp(int key);
int tick();
void draw();
int rnd(int min, int max);
extern bool paused;
extern byte goframe;
extern unsigned int keysheld[256];
extern int **imgframes;
extern int imgframes_count;
void init();
void EnableOpenGL();
void DisableOpenGL();
extern std::ofstream logfile;
void loadtexs();

#endif

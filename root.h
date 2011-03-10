#include "constants.h"

class Root {
    public:
        static int substate;
        static int state;
        static int CLOCKS_PER_FRAME;
        static class Game *game[8];
        static int numply;
        static int newnumply;
        static void KeyDown(int key);
        static void KeyUp(int key);
        static int tick();
        static void draw();
        static int rnd(int min, int max);
        static byte paused;
        static byte goframe;
        static unsigned int keysheld[256];
        static int **imgframes;
        static int imgframes_count;
    private:
        static byte rand_is_ready;
        static void Key(int key, byte repeat);
        static void initrand();
};
int Root::state = STATE_TITLE;
int Root::substate = 0;
byte Root::paused = 0;
byte Root::goframe = 1;
int Root::CLOCKS_PER_FRAME = 0;
Game *Root::game[8] = {0,0,0,0,0,0,0,0};
int Root::numply = 0;
int Root::newnumply = 1;
byte Root::rand_is_ready = 0;
unsigned int Root::keysheld[256] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};
int **Root::imgframes = 0;
int Root::imgframes_count = 0;

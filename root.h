#include "constants.h"
#include <fstream>

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
		static void init();
		static void EnableOpenGL();
		static void DisableOpenGL();
		static std::ofstream logfile;
    private:
        static byte rand_is_ready;
        static void Key(int key, byte repeat);
        static void initrand();
};

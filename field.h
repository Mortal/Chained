#include <gl/gl.h>
#include "constants.h"

class Field {
    public:
        GLfloat posx, posy;
        void swap(int x1, int y1, int x2, int y2);
        void rawswap(int x1, int y1, int x2, int y2);
        void move(int x1, int y1, int x2, int y2);
        static GLfloat calcpos(byte dimension, int blocks, int cellnum, int cells);
        void draw(GLfloat offx, GLfloat offy, GLfloat extray_draw, GLfloat extray_tex);
        int getwidth();
        int getheight();
        short getcolors();
        byte gethasvs();
        bool blockis(int x, int y);
        int blockface(int x, int y);
        int blockstate(int x, int y);
        byte blockischain(int x, int y);
        byte blockfallchain(int x, int y);
        void newfield(int width, int height, int colors, int hasvs);
        void newline();
        class Game *parentgame;
        void clearmatchframes();
        void checkblocks();
        void fallblocks();
        Field();
        void deleteblock(int x, int y);
        int swapable(int x, int y);
        
        int curchain;
        
        // The rising stack
        void raisestack();
        byte canraisestack;
        byte speedrising_key;
        byte speedrising;
        int stoptime;
        int stackoffset;
        int stacktick;
    private:
        int allocnextrow();
        class Block ** blocks;
        class Block ** nextrow;
        byte nextrowfilled;
        int width, height;
        short colors;
        byte hasvs;
        class SwapStack *swapStack;
        class Popper *poppers[MAXPOPPERS];
        int usedpoppers;
        int notStillCount();
};

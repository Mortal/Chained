#include <gl/gl.h>

class Game {
    public:
        class Field field;
        int character;
        int cursorx; int cursory;
        int points;
        class ScoringSystem ss;
        void draw();
        void key(int which, byte dir);
        Game() {cursorx = 2; cursory = 4;}
        void drawbackground(GLfloat offx, GLfloat offy);
        void drawcursors(GLfloat offx, GLfloat offy);
        //class Root *parent; will always be static
        int id;
};

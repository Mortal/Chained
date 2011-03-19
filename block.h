#include <GL/gl.h>
#include "constants.h"

class Block {
    public:
        int face;
        int facestate;
        int state;
        int substate; // Framecount
        int ischain;
        int matchframe;
        Block() {
            this->face = 0;
            this->facestate = FACESTATE_NORMAL;
            this->state = 0;
            this->substate = 0;
            this->ischain = 0;
        }
        Block(Block * block) {
            this->face = block->face;
            this->facestate = block->facestate;
            this->state = block->state;
            this->substate = block->substate;
            this->ischain = block->ischain;
        }
        void draw(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat extray_draw, GLfloat extray_tex);
        int swapable();
};

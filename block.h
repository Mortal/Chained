#include <GL/gl.h>
#include "constants.h"
#include "root.h"

class Block {
	public:
		int face;
		int facestate;
		int state;
		int substate; // Framecount
		int ischain;
		int matchframe;
		inline Block() {
			this->face = 0;
			this->facestate = FACESTATE_NORMAL;
			this->state = 0;
			this->substate = 0;
			this->ischain = 0;
		}
		inline Block(Block * block) {
			this->face = block->face;
			this->facestate = block->facestate;
			this->state = block->state;
			this->substate = block->substate;
			this->ischain = block->ischain;
		}
		inline void draw(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat extray_draw, GLfloat extray_tex) {
			if (this->face == BLOCKFACE_NOWT || this->face == BLOCKFACE_GARBAGE) return;
			if DEBUGIT_BLOCKDRAW logfile << "Block::draw	 ---- " << x << " " << y << " " << w << " " << h << std::endl;
			int face = 0;
			if (this->facestate == FACESTATE_NORMAL) face += IMGMEM_OFFSET0;
			else if (this->facestate == FACESTATE_BLINK) face += IMGMEM_OFFSETA;
			else if (this->facestate == FACESTATE_CLEAR) face += IMGMEM_OFFSETB;
			else face = 0;
			face += IMGMEM__FRAMES * this->face;
			if DEBUGIT_BLOCKDRAW logfile << "Block::draw	 Face = " << face << std::endl;
			glBindTexture (GL_TEXTURE_2D, face);
			y += h; x += w;
			glBegin (GL_QUADS);
			glTexCoord2f (0.0f, 0.0f);
			glVertex2f (x, y); // NE
			float oldy = y;
			y -= h;
			glTexCoord2f (0.0f, 1.0f-extray_tex);
			glVertex2f (x, y-((-extray_tex)*BLOCK_HEIGHT/(float) SCREEN_HEIGHT*2)); // SE
			x -= w;
			glTexCoord2f (1.0f, 1.0f-extray_tex);
			glVertex2f (x, y-((-extray_tex)*BLOCK_HEIGHT/(float) SCREEN_HEIGHT*2)); // SW
			y = oldy;
			glTexCoord2f (1.0f, 0.0f);
			glVertex2f (x, y); // NW
			glEnd();
		}
		int swapable();
};

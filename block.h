/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_BLOCK
#define CHAINED_BLOCK
#include <GL/gl.h>
#include "constants.h"
#include "root.h"
#include "textures.h"

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
			if (DEBUGIT_BLOCKDRAW) logfile << "Block::draw	 ---- " << x << " " << y << " " << w << " " << h << std::endl;
			bind_texture(this->face, this->facestate);
			y += h; x += w;
			static float colors[][4] = {
				{ 0.8f, 0.0f, 0.0f, 1, },
				{ 0.8f, 0.8f, 0.0f, 1, },
				{ 0.0f, 0.8f, 0.0f, 1, },
				{ 0.0f, 0.8f, 0.8f, 1, },
				{ 0.8f, 0.0f, 0.8f, 1, },
				{ 0.0f, 0.0f, 0.8f, 1, },
				{ 1.0f, 1.0f, 1.0f, 1, },
			};
			float * color = colors[this->face % 6];
			if (this->facestate == FACESTATE_BLINK) color = colors[6];
			bool tex = true;
			glBegin (GL_QUADS);
			if (tex) glTexCoord2f (0.0f, 0.0f);
			else glColor4fv(color);
			glVertex2f (x, y); // NE
			float oldy = y;
			y -= h;
			if (tex) glTexCoord2f (0.0f, 1.0f-extray_tex);
			glVertex2f (x, y-((-extray_tex)*BLOCK_HEIGHT/(float) SCREEN_HEIGHT*2)); // SE
			x -= w;
			if (tex) glTexCoord2f (1.0f, 1.0f-extray_tex);
			glVertex2f (x, y-((-extray_tex)*BLOCK_HEIGHT/(float) SCREEN_HEIGHT*2)); // SW
			y = oldy;
			if (tex) glTexCoord2f (1.0f, 0.0f);
			glVertex2f (x, y); // NW
			glEnd();
		}
		int swapable() {
			if (this->face == BLOCKFACE_NOWT || this->face == BLOCKFACE_VS || !(this->state == BLOCKSTATE_STILL || this->state == BLOCKSTATE_SWITCHING || this->state == BLOCKSTATE_WILLFALL)) {
				return 0;
			}
			return 1;
		}
};
#endif

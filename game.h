/* vim:set ts=4 sw=4 sts=4 noet: */
#include <GL/gl.h>
#include "root.h"

class Game {
	public:
		class Field field;
		int character;
		int cursorx; int cursory;
		int points;
		class ScoringSystem ss;
		void draw() {
			GLfloat offx = this->field.posx;
			GLfloat offy = this->field.posy;

			this->drawbackground(offx, offy);

			GLfloat extray_tex = (float) this->field.stackoffset/(float) STACKOFFSET_MAX;
			GLfloat extray_draw = (extray_tex*BLOCK_HEIGHT/(float) SCREEN_HEIGHT*2);
			offy += extray_draw;

			this->field.draw(offx, offy, extray_draw, extray_tex);

			this->drawcursors(offx, offy);
		}
		inline void key(int which, byte dir) {
			if DEBUGIT_KEYS logfile << "Game::key	  Game " << which << " direction " << (int) dir << std::endl;
			switch (which) {
				case GAMEKEY_LEFT:
					if (dir == KEYTYPE_UP) break;
					if (this->cursorx > 0) this->cursorx--;
					break;
				case GAMEKEY_UP:
					if (dir == KEYTYPE_UP) break;
					if (this->cursory > 0) this->cursory--;
					break;
				case GAMEKEY_RIGHT:
					if (dir == KEYTYPE_UP) break;
					if ((this->cursorx+1) < this->field.getwidth()-1) this->cursorx++;
					break;
				case GAMEKEY_DOWN:
					if (dir == KEYTYPE_UP) break;
					if ((this->cursory) < this->field.getheight()-1) this->cursory++;
					break;
				case GAMEKEY_SWITCH:
					if (dir != KEYTYPE_DOWN) break;
					this->field.swap(this->cursorx, this->cursory, this->cursorx+1, this->cursory);
					break;
				case GAMEKEY_STACK:
					if (dir == KEYTYPE_DOWN) {
						this->field.stoptime = 0;
						this->field.speedrising_key = 1;
					} else if (dir == KEYTYPE_UP) {
						this->field.speedrising_key = 0;
					}
					break;
			}
}
		Game() {cursorx = 2; cursory = 4;}
		void drawbackground(GLfloat offx, GLfloat offy);
		void drawcursors(GLfloat offx, GLfloat offy);
		//class Root *parent; will always be static
		int id;
};

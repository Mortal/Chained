/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_SWAP
#define CHAINED_SWAP
#include "constants.h"

class Swap {
	public:
		class Block *block;
		byte switchblock;
		class Field *parentfield;
		int fromx,fromy,tox,toy;
		Swap(Block *block_, int fromx_, int fromy_, int tox_, int toy_, byte switchblock_, class Field *parentfield) {
			this->block = block_;
			this->fromx = fromx_;
			this->fromy = fromy_;
			this->tox = tox_;
			this->toy = toy_;
			this->switchblock = switchblock_;
			this->parentfield = parentfield;
			this->frame = 0;
			if (this->switchblock) {
				if DEBUGIT_SWAPCONSTRUCT logfile << "Swap::Swap	  Rawswapping (" << (int) this->switchblock << ") " << fromx << "," << fromy << " " << tox << "," << toy << std::endl;
				this->parentfield->rawswap(fromx,fromy,tox,toy);
			} else {
				if DEBUGIT_SWAPCONSTRUCT logfile << "Swap::Swap	  Not rawswapping (" << (int) this->switchblock << ") " << fromx << "," << fromy << " " << tox << "," << toy << std::endl;
			}
		}
		int draw(float offx, float offy) {
			int h = this->parentfield->getheight();
			float pos1x = 2*((GLfloat) (BLOCK_WIDTH * fromx) / (GLfloat) SCREEN_WIDTH);
			float pos1y = 2*((GLfloat) (BLOCK_HEIGHT * (h-fromy-1)) / (GLfloat) SCREEN_HEIGHT);
			float pos2x = 2*((GLfloat) (BLOCK_WIDTH * tox) / (GLfloat) SCREEN_WIDTH);
			float pos2y = 2*((GLfloat) (BLOCK_HEIGHT * (h-toy-1)) / (GLfloat) SCREEN_HEIGHT);
			pos1x *= (float) ((TIME_SWITCH-1)-this->frame) / (float) (TIME_SWITCH-1);
			pos1y *= (float) ((TIME_SWITCH-1)-this->frame) / (float) (TIME_SWITCH-1);
			pos2x *= (float) this->frame / (float) (TIME_SWITCH-1);
			pos2y *= (float) this->frame / (float) (TIME_SWITCH-1);
			offx += pos1x+pos2x;
			offy += pos1y+pos2y;
			this->block->draw((GLfloat) offx, (GLfloat) offy, (GLfloat) 2*BLOCK_WIDTH / SCREEN_WIDTH, (GLfloat) 2*BLOCK_HEIGHT / SCREEN_HEIGHT, 0, 0);
			if (++this->frame >= TIME_SWITCH) {
				this->block->state = BLOCKSTATE_STILL;
				return 1;
			}
			return 0;
		}
	private:
		int frame;
};
#endif

/* vim:set ts=4 sw=4 sts=4 noet: */
#include "field.h"
#include "swapstack.h"
#include "game.h"

void Field::raisestack() {
	byte raisenow = 0;
	// whether we may raise the stack
	byte stackrising = (swapStack->swapCount() == 0) && (notStillCount() == 0);
	
	// Check if we're actually gonna raise the stack
	if (stackrising) {
		if (this->speedrising) {
			raisenow = 1;
		}
		if DEBUGIT_FIELDRAISESTACK logfile << "Field::raisest. Speedrising = " << (this->speedrising ? 1 : 0) << std::endl;
	}
	
	// Raise the stack if we're going to
	if (raisenow) {
		if DEBUGIT_FIELDRAISESTACK logfile << "Field::raisest. Oh god we're rising now." << std::endl;
		if (++this->stackoffset > STACKOFFSET_MAX) {
			this->stackoffset = 0;
			this->speedrising = 0;
			int firstrow = 1;
			int w = this->getwidth();
			int h = this->getheight();
			for (int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					int idx = y * w + x;
					int idxabove = idx-w;
					if (idxabove<0) {
						if (this->blocks[idx]) {
							delete this->blocks[idx];
						}
						this->blocks[idx] = 0;
					} else {
						this->blocks[idxabove] = this->blocks[idx];
					}
				}
			}
			int idx = (h-1) * w;
			for (int x = 0; x < w; ++x) {
				Block * curblock = this->blocks[idx] = new Block(this->nextrow[x]);
				curblock->state = BLOCKSTATE_STILL;
				++idx;
			}
			this->newline();
			if (this->parentgame->cursory) this->parentgame->cursory--;
		}
	}
	
	if (stackrising && this->speedrising_key) {
		if DEBUGIT_FIELDRAISESTACK logfile << "Field::raisest. Gonna speedrise (key)" << std::endl;
		this->speedrising = 1;
	}
}

void Field::draw(GLfloat offx, GLfloat offy, GLfloat extray_draw, GLfloat extray_tex) {
	int blockx = 0;
	int blocky = 0;
	GLfloat blockw = (GLfloat) 2*BLOCK_WIDTH / SCREEN_WIDTH;
	GLfloat blockh = (GLfloat) 2*BLOCK_HEIGHT / SCREEN_HEIGHT;
	if DEBUGIT_FIELDDRAW logfile << "Field::draw	 Drawing field " << this->parentgame->id << "! " << offx << " " << offy << " " << extray_draw << " " << extray_tex << std::endl;
	int height = this->getheight();
	int width = this->getwidth();
	for (int i = 0; i < height*width; i++) {
		while (blockx >= width) {blocky++; blockx -= width;}
		if (this->blocks[i] && this->blocks[i]->state != BLOCKSTATE_SWITCHING && this->blocks[i]->state != BLOCKSTATE_POPPING) {
			GLfloat blockoffx = offx + 2*((GLfloat) (BLOCK_WIDTH * blockx) / (GLfloat) SCREEN_WIDTH);
			GLfloat blockoffy = offy + 2*((GLfloat) (BLOCK_HEIGHT * (height-blocky-1)) / (GLfloat) SCREEN_HEIGHT);
			this->blocks[i]->draw(blockoffx, blockoffy, blockw, blockh, 0, 0);
		}
		blockx++;
	}
	swapStack->draw(offx, offy);
	for (int i = 0; i < this->usedpoppers; i++) {
		if (!this->poppers[i]) continue;
		if (this->poppers[i]->draw(offx,offy)) {
			this->poppers[i]->clean();
			delete this->poppers[i];
			this->poppers[i] = 0;
		}
	}
	for (int i = 0; i < width; ++i) {
		GLfloat blockoffx = offx + 2*((GLfloat) (BLOCK_WIDTH * i) / (GLfloat) SCREEN_WIDTH);
		GLfloat blockoffy = offy - 2*((GLfloat) BLOCK_HEIGHT / (GLfloat) SCREEN_HEIGHT);
		this->nextrow[i]->draw(blockoffx, blockoffy, blockw, blockh, extray_draw, 1-extray_tex);
	}
}

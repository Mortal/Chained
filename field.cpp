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

void Field::swap(int x1, int y1, int x2, int y2) {
	if DEBUGIT_FIELDSWAP logfile << "Field::swap	 " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	int w = getwidth();
	int idx1 = y1*w+x1;
	int idx2 = y2*w+x2;
	Block *Block1 = this->blocks[idx1];
	Block *Block2 = this->blocks[idx2];
	if ((!Block1 && !Block2) || !(swapable(x1,y1) && swapable(x2,y2))) return;
	if (!Block1 && Block2) {
		if DEBUGIT_FIELDSWAP logfile << "Field::swap	 We're moving a block from left to right, right space is currently empty." << std::endl;
		this->swapStack->swap(Block2, x2,y2,x1,y1, 1, this);
	} else if (Block1 && !Block2) {
		if DEBUGIT_FIELDSWAP logfile << "Field::swap	 We're moving a block from right to left, left space is currently empty." << std::endl;
		this->swapStack->swap(Block1, x1,y1,x2,y2, 1, this);
	} else if (Block1 && Block2) {
		if DEBUGIT_FIELDSWAP logfile << "Field::swap	 We're switching two blocks." << std::endl;
		this->swapStack->swap(Block1, x1,y1,x2,y2, 0, this);
		this->swapStack->swap(Block2, x2,y2,x1,y1, 1, this);
	} else {
		logfile << "Field::swap	 -=-WARNING-=- We got in the else-part!" << std::endl;
	}
}

void Field::rawswap(int x1, int y1, int x2, int y2) {
	int i = 0;
	if (y2 != y1) {
		i++; // BREAK
	}
	if DEBUGIT_FIELDRAWSWAP logfile << "Field::rawswap  " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	int w = this->getwidth();
	int idx1 = y1*w+x1;
	int idx2 = y2*w+x2;
	Block *tempblock;
	if DEBUGIT_FIELDRAWSWAP logfile << "Field::rawswap  -- " << idx1 << " " << this->blocks[idx1] << " <-> " << idx2 << " " << this->blocks[idx2] << std::endl;
	tempblock = this->blocks[idx1];
	this->blocks[idx1] = this->blocks[idx2];
	this->blocks[idx2] = tempblock;
	if DEBUGIT_FIELDRAWSWAP logfile << "Field::rawswap  ++ " << idx1 << " " << this->blocks[idx1] << " <-> " << idx2 << " " << this->blocks[idx2] << std::endl;
	//if DEBUGIT_FIELDRAWSWAP logfile << "Field::rawswap  No crash!" << std::endl;
}

void Field::newfield(int width, int height, int colors, int hasvs) {
	this->width = width;
	this->height = height;
	this->colors = colors;
	this->hasvs = hasvs;
	int count = width * height;
	this->blocks = new Block*[width*height];
	for (int i = 0; i < count; i++) this->blocks[i] = 0;
	int startrows;
	if (height > 8) startrows = height - 4;
	else startrows = height/2;
	Block *curblock, *blockbeneath, *blockleft;
	for (int j = 0; j < startrows; j++) {
		for (int i = 0; i < width; i++) {
			int newidx = (width*(height-j-1))+i;
			if (this->blocks[newidx]) {
				logfile << "Field::newfield Hmm! Field->blocks[" << newidx << "] already set!" << std::endl;
				delete this->blocks[newidx];
				this->blocks[newidx] = 0;
			}
			curblock = this->blocks[newidx] = new Block;
			if (i == 0) blockleft = 0;
			else blockleft = this->blocks[newidx-1];
			if (j == 0) blockbeneath = 0;
			else {
				blockbeneath = this->blocks[newidx+width];
			}
			int color = rnd(1, colors-(blockleft?1:0)-(blockbeneath?1:0));
			if DEBUGIT_NEWFIELD {
				logfile << "Field::newfield " << i << "," << j << " " << color << " ";
				if (blockbeneath || blockleft) {
					logfile << "(";
					if (blockleft) {
						logfile << "left " << blockleft->face;
						if (blockbeneath) logfile << ", ";
					}
					if (blockbeneath) logfile << "under " << blockbeneath->face;
					logfile << ") ";
				}
			}
			if (blockleft && blockbeneath) {
				if	  (color >= blockleft->face && color >= blockbeneath->face) color += 2;
				else if (color >= blockleft->face) {
					color += 1;
					if (color >= blockbeneath->face) color += 1;
				} else if (color >= blockbeneath->face) {
					color += 1;
					if (color >= blockleft->face) color += 1;
				}
			} else if 
					((blockleft && (color >= blockleft->face)) ||
					(blockbeneath && (color >= blockbeneath->face))) {
				color += 1;
			}
			if DEBUGIT_NEWFIELD logfile << "Field::newfield " << color << std::endl;
			curblock->face = color;
			curblock->state = BLOCKSTATE_STILL;
			curblock->ischain = 0;
		}
	}
	newline();
}

Field::Field() {
	this->usedpoppers = 0;
	this->nextrowfilled = 0;
	this->nextrow = 0;
	
	this->curchain = 1;
	
	this->canraisestack = 0;
	this->speedrising_key = 0;
	this->speedrising = 0;
	this->stoptime = 0;
	this->stackoffset = 0;
	this->stacktick = 0;
	
	this->swapStack = new SwapStack;
}

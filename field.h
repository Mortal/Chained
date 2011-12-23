/* vim:set ts=4 sw=4 sts=4 noet: */
#include <GL/gl.h>
#include "constants.h"
#include "block.h"
#include "popper.h"

class Field {
	public:
		GLfloat posx, posy;
		void swap(int x1, int y1, int x2, int y2);
		void rawswap(int x1, int y1, int x2, int y2);
		void move(int x1, int y1, int x2, int y2);
		static GLfloat calcpos(byte dimension, int blocks, int cellnum, int cells) {
			if (cells <= 0) return (GLfloat) 1.0f;
			int screenlen = (dimension ? SCREEN_HEIGHT : SCREEN_WIDTH);
			int sidelen = (dimension ? BLOCK_HEIGHT : BLOCK_WIDTH) * blocks;
			int space = (screenlen-(cells*sidelen)) / (1+cells);
			if (space < 0) space = 0;
			//cout << "  " << screenlen << " " << sidelen << " " << space << endl;
			return ((GLfloat) ((space * cellnum) + (sidelen * (cellnum-1))) / (GLfloat) screenlen)*2-1;
		}
		void draw(GLfloat offx, GLfloat offy, GLfloat extray_draw, GLfloat extray_tex);
		int getwidth() { return this->width; }
		int getheight() { return this->height; }
		short getcolors() { return this->colors; }
		byte gethasvs() { return this->hasvs; }
		bool blockis(int x, int y) {
			int idx = y*getwidth()+x;
			if (this->blocks[idx]) return true;
			return false;
		}
		int blockface(int x, int y) {
			int idx = y*getwidth()+x;
			if (this->blocks[idx]) return this->blocks[y*getwidth()+x]->face;
			return BLOCKFACE_NOWT;
		}
		int blockstate(int x, int y) {
			int idx = y*getwidth()+x;
			if (this->blocks[idx]) return this->blocks[y*getwidth()+x]->state;
			return BLOCKSTATE_SHH;
		}
		byte blockischain(int x, int y) {
			int idx = y*getwidth()+x;
			if (this->blocks[idx]) return this->blocks[y*getwidth()+x]->ischain ? 1 : 0;
			return 0;
		}
		byte blockfallchain(int x, int y) {
			if (!blockis(x,y)) return 0;
			switch (blockstate(x,y)) {
				case BLOCKSTATE_STILL:
				case BLOCKSTATE_WILLFALL:
					break;
				default:
					return 0;
			}
			int idx = y*getwidth()+x;
			if (!blocks[idx]) logfile << "Field::bl.fl.ch " << __LINE__ << ": We're gonna crash." << std::endl;
			blocks[idx]->state = BLOCKSTATE_WILLFALL;
			blocks[idx]->substate = 0;
			blocks[idx]->ischain = true;
			return 1;
		}
		void newfield(int width, int height, int colors, int hasvs);
		void newline() {
			if (!canraisestack) return;
			int w = getwidth();
			int h = getheight();
			if (!allocnextrow()) {
				for (int i = 0; i < w; i++) {
					if (nextrow[i]) {
						delete nextrow[i];
						nextrow[i] = 0;
					}
				}
			}
			int lastface = -1;
			int *watchout = new int[w];
			for (int i = 0; i < w; i++) {
				int idx1 = (h-1)*w+i;
				int idx2 = (h-2)*w+i;
				if ((!blocks[idx1]) || (!blocks[idx2])) {
					watchout[i] = -1;
				} else if (blocks[idx1]->face == blocks[idx2]->face) {
					watchout[i] = blocks[idx1]->face;
				}
			}
			for (int i = 0; i < w; i++) {
				nextrow[i] = new Block;
				nextrow[i]->state = BLOCKSTATE_WAITING;
				int count = colors;
				if (lastface > -1) count--;
				if (watchout[i] > -1) count--;
				int newface = rnd(1, count);
				if (lastface > -1 && watchout[i] > -1) {
					if	  (newface >= lastface && newface >= watchout[i]) newface += 2;
					else if (newface >= lastface) {
						newface += 1;
						if (newface >= watchout[i]) newface += 1;
					} else if (newface >= watchout[i]) {
						newface += 1;
						if (newface >= lastface) newface += 1;
					}
				} else if 
					((lastface > -1 && (newface >= lastface)) ||
					 (watchout[i] > -1 && (newface >= watchout[i]))) {
						newface += 1;
					}
				if (DEBUGIT_NEWLINE) logfile << "Field::newline  Face " << i << ": " << newface << std::endl;
				nextrow[i]->face = lastface = newface;
			}
			delete watchout;

		}
		class Game *parentgame;
		void clearmatchframes() {
			int w = this->getwidth();
			int h = this->getheight(); 
			for (int y = h; y--;) {
				for (int x = 0; x < w; x++) {
					if (this->blocks[y*w+x]) this->blocks[y*w+x]->matchframe = 0;
				}
			}   
		}
		void DOYOURTHING(int blocksDIR, int COORD, int DIR, int max, bool horizontal, const char * DEBUGMSG, int & ischain, int & totalmatches, int w, int h, Block * cur, int x, int y) {
			if (blocksDIR > 1) {
				int matches = 0;
				for (int i = (COORD) + (DIR); i >= 0 && i < max; i += (DIR)) {
					const int IDX = horizontal ? y * w + i : i * w + x;
					if (
						(!this->blocks[IDX]) ||
						(!(this->blocks[IDX]->state == BLOCKSTATE_STILL)) ||
						(!(this->blocks[IDX]->face == cur->face))
					   ) break;
					if (IDX+w<w*h) {
						if (
							(!this->blocks[IDX+w])
						   ) break;
					}
					if (this->blocks[IDX]->state != BLOCKSTATE_STILL) logfile << "aah jerv" << std::endl;
					ischain |= this->blocks[IDX]->ischain;
					matches++;
				}
				if (matches > 1) {
					if (DEBUGIT_FIELDCHECKBLOCKS) logfile << "Field::chkblo.  " << x << "," << y << " Matching " << DEBUGMSG << "!" << std::endl;
					for (int i = COORD; i >= COORD + ((DIR) * matches); i += (DIR)) {
						const int IDX = horizontal ? y * w + i : i * w + x;
						if (!this->blocks[IDX]->matchframe) {
							this->blocks[IDX]->matchframe = 1;
							totalmatches++;
						}
					}
				}
			}
		}
		void checkblocks() {
			int w = this->getwidth();
			int h = this->getheight();
			int firstrow = 1;
			this->clearmatchframes();
			int ischain = 0;
			int totalmatches = 0;
			for (int y = h; y--;) {
				for (int x = 0; x < w; x++) {
					Block *cur = this->blocks[y*w+x];
					if (cur && cur->state == BLOCKSTATE_STILL) {
						int blocksleft = x;
						int blocksright = w-x-1;
						int blocksbelow = y;
						int blocksabove = h-y-1;
						//if DEBUGIT_FIELDCHECKBLOCKS logfile << "Field::chkblo.  " << x << "," << y << " Blocks left,right,above,below = " << blocksleft << "," << blocksright << "," << blocksabove << "," << blocksbelow << std::endl;

						DOYOURTHING(blocksleft, x, -1, w, true, "to the left", ischain, totalmatches, w, h, cur, x, y);
						DOYOURTHING(blocksright, x, 1, w, true, "to the right", ischain, totalmatches, w, h, cur, x, y);
						DOYOURTHING(blocksbelow, y, -1, h, false, "below", ischain, totalmatches, w, h, cur, x, y);
						DOYOURTHING(blocksabove, y, -1, h, false, "above", ischain, totalmatches, w, h, cur, x, y);
					}
				}
				firstrow = 0;
			}
			if (totalmatches) {
				if (DEBUGIT_FIELDCHECKBLOCKS) logfile << "Field::chkblo.  Alright, we have " << totalmatches << " matches." << std::endl;
				int popperid = -1;
				for (int i = 0; i < this->usedpoppers; i++) {
					if (!this->poppers[i]) {popperid = i; break;}
				}
				if (popperid < 0) {
					popperid = this->usedpoppers++;
				}
				if (popperid < MAXPOPPERS) {
					if (DEBUGIT_FIELDCHECKBLOCKS) logfile << "Field::chkblo.  popperid = " << popperid << std::endl;
					Popper *popper;
					popper = new Popper(totalmatches, this);
					for (int y = 0; y < h; y++) {
						for (int x = 0; x < w; x++) {
							if (this->blocks[y*w+x] && this->blocks[y*w+x]->matchframe) {
								if (DEBUGIT_FIELDCHECKBLOCKS) logfile << "Field::chkblo.  Adding " << x << "," << y << std::endl;
								popper->addblock(this->blocks[y*w+x], x, y);
							}
						}
					}
					this->poppers[popperid] = popper;
				}
			}
		}
		void fallblocks() {
			int w = this->getwidth();
			int h = this->getheight();
			int firstrow = 1;
			int fallen = 0; // How many blocks have fallen this frame.
			for (int y = h; y--;) {
				for (int x = 0; x < w; x++) {
					Block *cur = this->blocks[y*w+x];
					Block *beneath = 0x0;
					if (!firstrow) beneath = this->blocks[(y+1)*w+x];
					if (this->blockface(x,y) != BLOCKFACE_NOWT) {
						switch (this->blockstate(x,y)) {
							case BLOCKSTATE_STILL:
								if (!firstrow) {
									if (!beneath) {
										if (DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. " << x << "," << y << " We're gonna fall!" << std::endl;
										cur->state = BLOCKSTATE_WILLFALL;
										cur->substate = 0;
									} else if (beneath->state == BLOCKSTATE_WILLFALL || beneath->state == BLOCKSTATE_FALLING) {
										if (DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. " << x << "," << y << " We're gonna fall along with the block beneath us." << std::endl;
										cur->state = BLOCKSTATE_WILLFALL;
										cur->substate = ((beneath->state == BLOCKSTATE_WILLFALL) ? beneath->substate : 0);
									}
								}
								break;
							case BLOCKSTATE_WILLFALL:
								if (!firstrow) {
									if (beneath && beneath->state != BLOCKSTATE_FALLING && beneath->state != BLOCKSTATE_WILLFALL) {
										if (DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. " << x << "," << y << " Apparently, we can't fall anyway, something's beneath us." << std::endl;
										cur->state = BLOCKSTATE_STILL;
										cur->substate = 0;
									} else if (++cur->substate >= TIME_FALL_BEFORE) {
										if (DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. " << x << "," << y << " We're falling!" << std::endl;
										cur->state = BLOCKSTATE_FALLING;
										cur = 0;
									}
								}
								break;
							case BLOCKSTATE_FALLING:
								if (beneath || firstrow) {
									if (DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. " << x << "," << y << " Plonk. Stopped falling." << std::endl;
									cur->state = BLOCKSTATE_STILL;
									cur->substate = 0;
								} else {
									if (DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. " << x << "," << y << " Falling a step." << std::endl;
									this->rawswap(x,y,x,y+1);
									fallen++;
									cur = 0;
								}
								break;
						}
					}
				}
				firstrow = 0;
			}
			if (fallen && DEBUGIT_FIELDFALLBLOCKS) logfile << "Field::fallblo. Blocks fallen: " << fallen << std::endl;
		}
		Field();
		void deleteblock(int x, int y) {
			int w = this->getwidth();
			int idx = y*w+x;
			if (this->blocks[idx]) {
				delete this->blocks[idx];
				this->blocks[idx] = 0;
			}
		}
		int swapable(int x, int y) {
			int w = getwidth();
			int h = getheight();
			int idx = y * w + x;
			if (!blocks[idx]) {
				if (DEBUGIT_FIELDSWAPABLE) logfile << "Field::swapable " << x << "," << y << " doesn't exist!" << std::endl;
				if (y > 0) {
					int above = idx-w;
					if (blocks[above] && blocks[above]->state != BLOCKSTATE_FALLING) {
						if (DEBUGIT_FIELDSWAPABLE) logfile << "Field::swapable " << x << "," << y << " The block above us exists, so we can't be switched!" << std::endl;
						return 0;
					}
				}
				if (DEBUGIT_FIELDSWAPABLE) logfile << "Field::swapable " << x << "," << y << " ...but the aboves are clear so that doesn't matter!" << std::endl;
				return 1;
			} else {
				if (y+1 < h) {
					int below = idx+w;
					if (!blocks[below] || blocks[below]->state == BLOCKSTATE_SHH || blocks[below]->face == BLOCKFACE_NOWT) {
						if (DEBUGIT_FIELDSWAPABLE) logfile << "Field::swapable " << x << "," << y << " The block under us doesn't exist, so we can't be switched!" << std::endl;
						return 0;
					}
				}
				if (DEBUGIT_FIELDSWAPABLE) logfile << "Field::swapable " << x << "," << y << " We'll let the local block decide." << std::endl;
				return blocks[idx]->swapable();
			}
		}
		
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
		int allocnextrow() {
			if (this->nextrow) return 0;
			int w = this->getwidth();
			if (!w) return 0;
			this->nextrow = new Block*[this->getwidth()];
			return 1;
		}
		class Block ** blocks;
		class Block ** nextrow;
		byte nextrowfilled;
		int width, height;
		short colors;
		byte hasvs;
		class SwapStack *swapStack;
		class Popper *poppers[MAXPOPPERS];
		int usedpoppers;
		inline int notStillCount() {
			int count = 0;
			int n = getwidth();
			n *= getheight();
			for (int i = 0; i < n; i++) {
				if (blocks[i] && blocks[i]->state != BLOCKSTATE_STILL) {
					if (DEBUGIT_FIELDPOPPERCOUNT) logfile << "Field::fallC.   poppers[" << i << "] is in use and not still: " << poppers[i] << std::endl;
					count++;
				}
			}
			return count;
		}
};

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
		void clearmatchframes() {
			int w = this->getwidth();
			int h = this->getheight(); 
			for (int y = h; y--;) {
				for (int x = 0; x < w; x++) {
					if (this->blocks[y*w+x]) this->blocks[y*w+x]->matchframe = 0;
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

#define DOYOURTHING(blocksDIR, COORD, DIR, INCR, IDX, DEBUGMSG) \
						if (blocksDIR > 1) {\
							int matches = 0;\
							for (int i = (COORD) + (DIR); INCR; i += (DIR)) {\
								if (\
									(!this->blocks[IDX]) ||\
									(!(this->blocks[IDX]->state == BLOCKSTATE_STILL)) ||\
									(!(this->blocks[IDX]->face == cur->face))\
								   ) break;\
								if (IDX+w<w*h) {\
									if (\
										(!this->blocks[IDX+w])\
									   ) break;\
								}\
								if (this->blocks[IDX]->state != BLOCKSTATE_STILL) logfile << "aah jerv" << std::endl;\
								ischain |= this->blocks[IDX]->ischain;\
								matches++;\
							}\
							if (matches > 1) {\
								if DEBUGIT_FIELDCHECKBLOCKS logfile << "Field::chkblo.  " << x << "," << y << " Matching " << DEBUGMSG << "!" << std::endl;\
								for (int i = COORD; i >= COORD + ((DIR) * matches); i += (DIR)) {\
									if (!this->blocks[IDX]->matchframe) {\
										this->blocks[IDX]->matchframe = 1;\
										totalmatches++;\
									}\
								}\
							}\
						}
						DOYOURTHING(blocksleft, x, -1, i >= 0, y*w+i, "to the left");
						DOYOURTHING(blocksright, x, 1, i < w, y*w+i, "to the right");
						DOYOURTHING(blocksbelow, y, -1, i < h, i*w+x, "below");
						DOYOURTHING(blocksabove, y, -1, i >= 0, i*w+x, "above");
					}
				}
				firstrow = 0;
			}
			if (totalmatches) {
				if DEBUGIT_FIELDCHECKBLOCKS logfile << "Field::chkblo.  Alright, we have " << totalmatches << " matches." << std::endl;
				int popperid = -1;
				for (int i = 0; i < this->usedpoppers; i++) {
					if (!this->poppers[i]) {popperid = i; break;}
				}
				if (popperid < 0) {
					popperid = this->usedpoppers++;
				}
				if (popperid < MAXPOPPERS) {
					if DEBUGIT_FIELDCHECKBLOCKS logfile << "Field::chkblo.  popperid = " << popperid << std::endl;
					Popper *popper;
					popper = new Popper(totalmatches, this);
					for (int y = 0; y < h; y++) {
						for (int x = 0; x < w; x++) {
							if (this->blocks[y*w+x] && this->blocks[y*w+x]->matchframe) {
								if DEBUGIT_FIELDCHECKBLOCKS logfile << "Field::chkblo.  Adding " << x << "," << y << std::endl;
								popper->addblock(this->blocks[y*w+x], x, y);
							}
						}
					}
					this->poppers[popperid] = popper;
				}
			}
		}
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
		inline int notStillCount() {
			int count = 0;
			int n = getwidth();
			n *= getheight();
			for (int i = 0; i < n; i++) {
				if (blocks[i] && blocks[i]->state != BLOCKSTATE_STILL) {
					if DEBUGIT_FIELDPOPPERCOUNT logfile << "Field::fallC.   poppers[" << i << "] is in use and not still: " << poppers[i] << std::endl;
					count++;
				}
			}
			return count;
		}
};

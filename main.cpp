/* vim:set ts=4 sw=4 sts=4 noet: */
// Mortal's Tetris Attack clone, begun May 5th 2007.

#include "winmain.h"
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "constants.h"
#include "vkeys.h"
#include "scoringsystem.h"
#include "tga.h"

#include "block.h"
#include "clearingblock.h"
#include "field.h"
#include "game.h"
#include "garbage.h"
#include "popper.h"
#include "swap.h"
#include "swapstack.h"
#include "root.h"

int state = STATE_TITLE;
int substate = 0;
byte paused = 0;
byte goframe = 1;
int CLOCKS_PER_FRAME = 0;
Game *game[8] = {0,0,0,0,0,0,0,0};
int numply = 0;
int newnumply = 1;
byte rand_is_ready = 0;
#define KEYCOUNT (256)
unsigned int keysheld[KEYCOUNT] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};
int **imgframes = 0;
int imgframes_count = 0;
std::ofstream logfile("log");

void KeyDown(int key) {
	if (key >= KEYCOUNT) return;
	keysheld[key] = 1;
	Key(key, KEYTYPE_DOWN);
}

void KeyUp(int key) {
	if (key >= KEYCOUNT) return;
	keysheld[key] = 0;
	Key(key, KEYTYPE_UP);
}

void initrand() {
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
	rand_is_ready = 1;
}

int rnd(int min, int max) {
	if (!rand_is_ready) initrand();
	return (int) (rand() * ((float) (max-min+1) / (float) RAND_MAX)) + min;
}

void Key(int key, byte dir) {
	byte repeat = dir == KEYTYPE_REPEAT;
	int rootkey = 1;
	switch (state) {
		case STATE_TITLE:
			if (dir == KEYTYPE_UP) break;
			if (substate < 1000) substate = 1000;
			else if (substate < 2000) substate = 2000;
			else {state = STATE_MENU; substate = 0;}
		break;
		case STATE_MENU:
			if (dir == KEYTYPE_UP) break;
			
		break;
		case STATE_GAME:
			if DEBUGIT_KEYS logfile << "key	  " << ((char) (key & 0xFF)) << " direction " << ((int) (dir)) << std::endl;
			if (dir == KEYTYPE_DOWN) {
				switch (key) {
					case VK_F1:
						newnumply = 1;
						state = STATE_TITLE;
					break;
					case VK_F2:
						newnumply = 2;
						state = STATE_TITLE;
					break;
					case 'P':
						if (paused) paused = 0;
						else {
							goframe = 0;
							paused = 1;
						}
					break;
					case 'O':
						goframe = 1;
					break;
					default:
						rootkey = 0;
					break;
				}
			} else rootkey = 0;
			if (!rootkey) {
				switch (numply) {
					case 1:
						switch (key) {
							///// PLAYER ONE /////
							case VK_LEFT: //leftarrow
								game[0]->key(GAMEKEY_LEFT, dir);
							break;
							case VK_UP: //uparrow
								game[0]->key(GAMEKEY_UP, dir);
							break;
							case VK_RIGHT: //rightarrow
								game[0]->key(GAMEKEY_RIGHT, dir);
							break;
							case VK_DOWN: //downarrow
								game[0]->key(GAMEKEY_DOWN, dir);
							break;
							case 'X':
							case 'Z':
							case 'x':
							case 'z':
								game[0]->key(GAMEKEY_SWITCH, dir);
							break;
							case 'C':
							case 'c':
								game[0]->key(GAMEKEY_STACK, dir);
							break;
						}
					break;
	
					case 2:
						switch (key) {
							///// PLAYER ONE /////
							case 'S':
								game[0]->key(GAMEKEY_LEFT, dir);
							break;
							case 'E':
								game[0]->key(GAMEKEY_UP, dir);
							break;
							case 'F':
								game[0]->key(GAMEKEY_RIGHT, dir);
							break;
							case 'D':
								game[0]->key(GAMEKEY_DOWN, dir);
							break;
							case 'A':
							case 'Z':
								game[0]->key(GAMEKEY_SWITCH, dir);
							break;
							case 'Q':
								game[0]->key(GAMEKEY_STACK, dir);
							break;
							
							///// PLAYER TWO /////
							case VK_LEFT:
								game[1]->key(GAMEKEY_LEFT, dir);
							break;
							case VK_UP:
								game[1]->key(GAMEKEY_UP, dir);
							break;
							case VK_RIGHT:
								game[1]->key(GAMEKEY_RIGHT, dir);
							break;
							case VK_DOWN:
								game[1]->key(GAMEKEY_DOWN, dir);
							break;
							case 'M':
							case VK_OEM_COMMA:
								game[1]->key(GAMEKEY_SWITCH, dir);
							break;
							case VK_OEM_PERIOD: //full stop
								game[1]->key(GAMEKEY_STACK, dir);
							break;
							default:
								if (DEBUGIT_KEYS || DEBUGIT_KEYS_UNCAUGHT) {
									logfile << "Uncaught key: " << (char) key << " (" << (int) key << ")" << std::endl;
								}
							break;
						}
					break;
				}
			}
		break;
		case STATE_DEBUG:
			char *stuf = new char[256];
			sprintf(stuf, "Key: %c %u %#X", (key) & 0xFF, key, key);
			logfile << "key	  " << stuf << std::endl;
			delete stuf;
			stuf = 0;
		break;
	}
}

int tick() {
	if (paused) {
		if (!goframe) return 1;
		goframe = 0;
	}
	for (int i = 0; i < 256; i++) {
		if (keysheld[i] >= KEYTOHOLD) {
			Key(i, KEYTYPE_REPEAT);
		} else if (keysheld[i]) keysheld[i]++;
	}
	draw();
	switch (state) {
		case STATE_TITLE:
			for (int i = 0; i < numply; i++) {delete game[i]; game[i] = 0;}
			numply = newnumply;
			for (int i = 0; i < numply; i++) {
				game[i] = new Game;
				game[i]->id = i;
				game[i]->character = i;
				game[i]->points = 0;
				game[i]->field.newfield(FIELD_WIDTH,FIELD_HEIGHT,FIELD_COLORS,0);
				game[i]->field.canraisestack = 1;
				game[i]->field.newline();
				game[i]->field.parentgame = game[i];
				game[i]->field.posx = Field::calcpos(0, game[i]->field.getwidth(), i+1, numply);
				game[i]->field.posy = Field::calcpos(1, game[i]->field.getheight(), 1, 1);
			}
			state = STATE_GAME;
		break;
		case STATE_MENU:
			
		break;
		case STATE_GAME:
			for (int i = 0; i < numply; i++) {
				game[i]->field.raisestack();
				game[i]->field.checkblocks();
				game[i]->field.fallblocks();
			}
		break;
		case STATE_DEBUG:
			
		break;
	}
	return 0;
}

void draw() {
	switch (state) {
		case STATE_TITLE:
			
		break;
		case STATE_MENU:
			
		break;
		case STATE_GAME:
			if (numply) for (int i = 0; i < numply; i++) game[i]->draw();
			else {state = STATE_TITLE; substate = 0;}
		break;
		case STATE_DEBUG:
			for (int i = 1; i <= 8; i++) {
				int j = i;
				if (i >= 8) i += 1;
				glBindTexture (GL_TEXTURE_2D, i);
				glBegin (GL_QUADS);
				float x = 2*(-0.5f + ((1.0f+(2.0f*(j-(float) 1)))*(float) BLOCK_WIDTH)/(float) SCREEN_WIDTH);
				float y = 2*(0.5f - (float) BLOCK_HEIGHT/(float) SCREEN_HEIGHT);
				if DEBUGIT_ROOTDRAW logfile << "draw	  " << x << "," << y << " ";
				glTexCoord2f (0.0f, 0.0f);
				glVertex2f (x, y); // NW
				float oldy = y;
				y = 2*(0.5f - (float) 2*(float) BLOCK_HEIGHT/(float) SCREEN_HEIGHT);
				if DEBUGIT_ROOTDRAW logfile << "draw	  " << x << "," << y << " ";
				glTexCoord2f (0.0f, 1.0f);
				glVertex2f (x, y); // NE
				x = 2*(-0.5f + ((2.0f+(2.0f*(j-1.0f)))*(float) BLOCK_WIDTH)/(float) SCREEN_WIDTH);
				if DEBUGIT_ROOTDRAW logfile << "draw	  " << x << "," << y << " ";
				glTexCoord2f (1.0f, 1.0f);
				glVertex2f (x, y); // SE
				y = oldy;
				if DEBUGIT_ROOTDRAW logfile << "draw	  " << x << "," << y << std::endl;
				glTexCoord2f (1.0f, 0.0f);
				glVertex2f (x, y); // SW
				glEnd ();
			}
		break;
	}
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

int Field::allocnextrow() {
	if (this->nextrow) return 0;
	int w = this->getwidth();
	if (!w) return 0;
	this->nextrow = new Block*[this->getwidth()];
	return 1;
}

void Field::newline() {
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
		if DEBUGIT_NEWLINE logfile << "Field::newline  Face " << i << ": " << newface << std::endl;
		nextrow[i]->face = lastface = newface;
	}
	delete watchout;
	
}

GLfloat Field::calcpos(byte dimension, int blocks, int cellnum, int cells) {
	if (cells <= 0) return (GLfloat) 1.0f;
	int screenlen = (dimension ? SCREEN_HEIGHT : SCREEN_WIDTH);
	int sidelen = (dimension ? BLOCK_HEIGHT : BLOCK_WIDTH) * blocks;
	int space = (screenlen-(cells*sidelen)) / (1+cells);
	if (space < 0) space = 0;
	//cout << "  " << screenlen << " " << sidelen << " " << space << endl;
	return ((GLfloat) ((space * cellnum) + (sidelen * (cellnum-1))) / (GLfloat) screenlen)*2-1;
}

void init() {
	logfile.open("log");
	logfile << WINNAME << " " << VERSION << " log" << std::endl << "Rand_max: " << RAND_MAX << std::endl;
	state = STATE_TITLE;
	CLOCKS_PER_FRAME = (CLOCKS_PER_SEC / 80);
}

void loadtexs() {
	                 //012345678901234
	char filename[] = "block - -  .raw";
	for (int i = 1; i <= 9; ++i) {
		if (i == 8) continue;
		if (i < 10)
			filename[5] = (char) (i+'0');
		else
			filename[5] = (char) ((i-10)+'A');
		for (int j = 1; j <= 3; ++j) {
			switch (j) {
				case 1:
					filename[7] = '0';
				break;
				case 2:
					filename[7] = 'a';
				break;
				case 3:
					filename[7] = 'b';
				break;
				default:
					logfile << __LINE__ << " fffffff";
				break;
			}
			for (int frameid = 0; frameid < BLOCK_MAXFRAMES; ++frameid) {
				filename[9] = '0' + frameid / 10;
				filename[10] = '0' + frameid % 10;
				int imgidx = (j == 1 ? IMGMEM_OFFSET0 : (j == 2 ? IMGMEM_OFFSETA : IMGMEM_OFFSETB)) + IMGMEM__FRAMES * i + frameid;
				logfile << filename << "->" << imgidx << std::endl;
				std::fstream fin;
				fin.open(filename, std::ios::in | std::ios::binary);
				if (!fin.is_open()) break;
				
				int size = BLOCK_SRCWIDTH*BLOCK_SRCHEIGHT*3;
				char *image = new char[size];
				
				fin.read(image, size);
				fin.close();
			
				glBindTexture(GL_TEXTURE_2D, imgidx);
				
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
				
				glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, BLOCK_SRCWIDTH, BLOCK_SRCHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

				delete image;
				
			}
		}
	}
}

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
#include "root.h"

int Root::state = STATE_TITLE;
int Root::substate = 0;
byte Root::paused = 0;
byte Root::goframe = 1;
int Root::CLOCKS_PER_FRAME = 0;
Game *Root::game[8] = {0,0,0,0,0,0,0,0};
int Root::numply = 0;
int Root::newnumply = 1;
byte Root::rand_is_ready = 0;
unsigned int Root::keysheld[256] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};
int **Root::imgframes = 0;
int Root::imgframes_count = 0;
std::ofstream Root::logfile("log");

void Root::KeyDown(int key) {
	keysheld[key] = 1;
	Key(key, KEYTYPE_DOWN);
}

void Root::KeyUp(int key) {
	keysheld[key] = 0;
	Key(key, KEYTYPE_UP);
}

void Root::initrand() {
	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
	rand_is_ready = 1;
}

int Root::rnd(int min, int max) {
	if (!rand_is_ready) initrand();
	return (int) (rand() * ((float) (max-min+1) / (float) RAND_MAX)) + min;
}

void Root::Key(int key, byte dir) {
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
			if DEBUGIT_KEYS Root::logfile << "Root::key	  " << ((char) (key & 0xFF)) << " direction " << ((int) (dir)) << std::endl;
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
						if (Root::paused) Root::paused = 0;
						else {
							Root::goframe = 0;
							Root::paused = 1;
						}
					break;
					case 'O':
						Root::goframe = 1;
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
								game[0]->key(GAMEKEY_SWITCH, dir);
							break;
							case 'C':
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
									Root::logfile << "Uncaught key: " << (char) key << " (" << (int) key << ")" << std::endl;
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
			Root::logfile << "Root::key	  " << stuf << std::endl;
			delete stuf;
			stuf = 0;
		break;
	}
}

void Game::key(int which, byte dir) {
	if DEBUGIT_KEYS Root::logfile << "Game::key	  Game " << which << " direction " << (int) dir << std::endl;
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

int Root::tick() {
	if (Root::paused) {
		if (!Root::goframe) return 1;
		Root::goframe = 0;
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

int SwapStack::swapCount() {
	int count = 0;
	for (int i = 0; i < stacklen; i++) {
		if (stack[i]) {
			if DEBUGIT_SWAPSTACKSWAPCOUNT Root::logfile << "SwSt::swCount   stack[" << i << "] is in use: " << stack[i] << std::endl;
			count++;
		}
	}
	return count;
}

int Field::notStillCount() {
	int count = 0;
	int n = getwidth();
	n *= getheight();
	for (int i = 0; i < n; i++) {
		if (blocks[i] && blocks[i]->state != BLOCKSTATE_STILL) {
			if DEBUGIT_FIELDPOPPERCOUNT Root::logfile << "Field::fallC.   poppers[" << i << "] is in use and not still: " << poppers[i] << std::endl;
			count++;
		}
	}
	return count;
}

void Field::raisestack() {
	byte raisenow = 0;
	// whether we may raise the stack
	byte stackrising = (swapStack->swapCount() == 0) && (notStillCount() == 0);
	
	// Check if we're actually gonna raise the stack
	if (stackrising) {
		if (this->speedrising) {
			raisenow = 1;
		}
		if DEBUGIT_FIELDRAISESTACK Root::logfile << "Field::raisest. Speedrising = " << (this->speedrising ? 1 : 0) << std::endl;
	}
	
	// Raise the stack if we're going to
	if (raisenow) {
		if DEBUGIT_FIELDRAISESTACK Root::logfile << "Field::raisest. Oh god we're rising now." << std::endl;
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
		if DEBUGIT_FIELDRAISESTACK Root::logfile << "Field::raisest. Gonna speedrise (key)" << std::endl;
		this->speedrising = 1;
	}
}

void Field::clearmatchframes() {
	int w = this->getwidth();
	int h = this->getheight(); 
	for (int y = h; y--;) {
		for (int x = 0; x < w; x++) {
			if (this->blocks[y*w+x]) this->blocks[y*w+x]->matchframe = 0;
		}
	}   
}

void Field::checkblocks() {
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
				//if DEBUGIT_FIELDCHECKBLOCKS Root::logfile << "Field::chkblo.  " << x << "," << y << " Blocks left,right,above,below = " << blocksleft << "," << blocksright << "," << blocksabove << "," << blocksbelow << std::endl;
				
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
						if (this->blocks[IDX]->state != BLOCKSTATE_STILL) Root::logfile << "aah jerv" << std::endl;\
						ischain |= this->blocks[IDX]->ischain;\
						matches++;\
					}\
					if (matches > 1) {\
						if DEBUGIT_FIELDCHECKBLOCKS Root::logfile << "Field::chkblo.  " << x << "," << y << " Matching " << DEBUGMSG << "!" << std::endl;\
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
		if DEBUGIT_FIELDCHECKBLOCKS Root::logfile << "Field::chkblo.  Alright, we have " << totalmatches << " matches." << std::endl;
		int popperid = -1;
		for (int i = 0; i < this->usedpoppers; i++) {
			if (!this->poppers[i]) {popperid = i; break;}
		}
		if (popperid < 0) {
			popperid = this->usedpoppers++;
		}
		if (popperid < MAXPOPPERS) {
			if DEBUGIT_FIELDCHECKBLOCKS Root::logfile << "Field::chkblo.  popperid = " << popperid << std::endl;
			Popper *popper;
			popper = new Popper(totalmatches, this);
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++) {
					if (this->blocks[y*w+x] && this->blocks[y*w+x]->matchframe) {
						if DEBUGIT_FIELDCHECKBLOCKS Root::logfile << "Field::chkblo.  Adding " << x << "," << y << std::endl;
						popper->addblock(this->blocks[y*w+x], x, y);
					}
				}
			}
			this->poppers[popperid] = popper;
		}
	}
}

void Field::fallblocks() {
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
								if DEBUGIT_FIELDFALLBLOCKS Root::logfile << "Field::fallblo. " << x << "," << y << " We're gonna fall!" << std::endl;
								cur->state = BLOCKSTATE_WILLFALL;
								cur->substate = 0;
							} else if (beneath->state == BLOCKSTATE_WILLFALL || beneath->state == BLOCKSTATE_FALLING) {
								if DEBUGIT_FIELDFALLBLOCKS Root::logfile << "Field::fallblo. " << x << "," << y << " We're gonna fall along with the block beneath us." << std::endl;
								cur->state = BLOCKSTATE_WILLFALL;
								cur->substate = ((beneath->state == BLOCKSTATE_WILLFALL) ? beneath->substate : 0);
							}
						}
					break;
					case BLOCKSTATE_WILLFALL:
						if (!firstrow) {
							if (beneath && beneath->state != BLOCKSTATE_FALLING && beneath->state != BLOCKSTATE_WILLFALL) {
								if DEBUGIT_FIELDFALLBLOCKS Root::logfile << "Field::fallblo. " << x << "," << y << " Apparently, we can't fall anyway, something's beneath us." << std::endl;
								cur->state = BLOCKSTATE_STILL;
								cur->substate = 0;
							} else if (++cur->substate >= TIME_FALL_BEFORE) {
								if DEBUGIT_FIELDFALLBLOCKS Root::logfile << "Field::fallblo. " << x << "," << y << " We're falling!" << std::endl;
								cur->state = BLOCKSTATE_FALLING;
								cur = 0;
							}
						}
					break;
					case BLOCKSTATE_FALLING:
						if (beneath || firstrow) {
							if DEBUGIT_FIELDFALLBLOCKS Root::logfile << "Field::fallblo. " << x << "," << y << " Plonk. Stopped falling." << std::endl;
							cur->state = BLOCKSTATE_STILL;
							cur->substate = 0;
						} else {
							if DEBUGIT_FIELDFALLBLOCKS Root::logfile << "Field::fallblo. " << x << "," << y << " Falling a step." << std::endl;
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
	if (fallen && DEBUGIT_FIELDFALLBLOCKS) Root::logfile << "Field::fallblo. Blocks fallen: " << fallen << std::endl;
}

byte Field::blockfallchain(int x, int y) {
	if (!blockis(x,y)) return 0;
	switch (blockstate(x,y)) {
		case BLOCKSTATE_STILL:
		case BLOCKSTATE_WILLFALL:
		break;
		default:
		return 0;
	}
	int idx = y*getwidth()+x;
	if (!blocks[idx]) Root::logfile << "Field::bl.fl.ch " << __LINE__ << ": We're gonna crash." << std::endl;
	blocks[idx]->state = BLOCKSTATE_WILLFALL;
	blocks[idx]->substate = 0;
	blocks[idx]->ischain = true;
	return 1;
}

void Field::deleteblock(int x, int y) {
	int w = this->getwidth();
	int idx = y*w+x;
	if (this->blocks[idx]) {
		delete this->blocks[idx];
		this->blocks[idx] = 0;
	}
}

void Root::draw() {
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
				if DEBUGIT_ROOTDRAW Root::logfile << "Root::draw	  " << x << "," << y << " ";
				glTexCoord2f (0.0f, 0.0f);
				glVertex2f (x, y); // NW
				float oldy = y;
				y = 2*(0.5f - (float) 2*(float) BLOCK_HEIGHT/(float) SCREEN_HEIGHT);
				if DEBUGIT_ROOTDRAW Root::logfile << "Root::draw	  " << x << "," << y << " ";
				glTexCoord2f (0.0f, 1.0f);
				glVertex2f (x, y); // NE
				x = 2*(-0.5f + ((2.0f+(2.0f*(j-1.0f)))*(float) BLOCK_WIDTH)/(float) SCREEN_WIDTH);
				if DEBUGIT_ROOTDRAW Root::logfile << "Root::draw	  " << x << "," << y << " ";
				glTexCoord2f (1.0f, 1.0f);
				glVertex2f (x, y); // SE
				y = oldy;
				if DEBUGIT_ROOTDRAW Root::logfile << "Root::draw	  " << x << "," << y << std::endl;
				glTexCoord2f (1.0f, 0.0f);
				glVertex2f (x, y); // SW
				glEnd ();
			}
		break;
	}
}

void Game::draw() {
	GLfloat offx = this->field.posx;
	GLfloat offy = this->field.posy;

	this->drawbackground(offx, offy);
	
	GLfloat extray_tex = (float) this->field.stackoffset/(float) STACKOFFSET_MAX;
	GLfloat extray_draw = (extray_tex*BLOCK_HEIGHT/(float) SCREEN_HEIGHT*2);
	offy += extray_draw;
	
	this->field.draw(offx, offy, extray_draw, extray_tex);
	
	this->drawcursors(offx, offy);
}

void Game::drawbackground(GLfloat offx, GLfloat offy) {
	GLfloat w = offx;
	GLfloat n = offy;
	GLfloat e = w + (float) this->field.getwidth()*(float) BLOCK_WIDTH/(float) SCREEN_WIDTH*(float) 2;
	GLfloat s = n + (float) this->field.getheight()*(float) BLOCK_HEIGHT/(float) SCREEN_HEIGHT*(float) 2;
	
	//if DEBUGIT_DRAWBACKGROUND 
	
	glBegin(GL_QUADS);
		glColor4f(1.0f, ((this->id % 2) ? 1.0f : 0.0f), 0.0f, 1.0f);
		glVertex2f(w, n);
		glVertex2f(e, n);
		glVertex2f(e, s);
		glVertex2f(w, s);
	glEnd();
	
	if DEBUGIT_DRAWBACKGROUND Root::logfile << "Game::drawbg.   " << this->id << " " << n << " " << s << " " << w << " " << e << std::endl;
	if DEBUGIT_DRAWBACKGROUND Root::logfile << "Game::drawbg.   " << this->id << " " << n*SCREEN_HEIGHT << " " << s*SCREEN_HEIGHT << " " << w*SCREEN_WIDTH << " " << e*SCREEN_WIDTH << std::endl;
	if DEBUGIT_DRAWBACKGROUND Root::logfile << "Game::drawbg.   " << this->id << " " << (n-s)*SCREEN_HEIGHT << " " << (e-w)*SCREEN_WIDTH << std::endl;
}

void Game::drawcursors(GLfloat offx, GLfloat offy) {
	offx += (GLfloat) 2 * (this->cursorx * BLOCK_WIDTH) / (GLfloat) SCREEN_WIDTH;
	offy += (GLfloat) 2 * ((this->field.getheight()-this->cursory-1) * BLOCK_HEIGHT) / (GLfloat) SCREEN_HEIGHT;
	GLfloat topn = offy + (CURSOR_MARGIN / (GLfloat) SCREEN_HEIGHT);
	GLfloat tops = offy - ((CURSOR_PADDING) / (GLfloat) SCREEN_HEIGHT);
	offy += (GLfloat) 2 * (BLOCK_HEIGHT) / (GLfloat) SCREEN_HEIGHT;
	GLfloat bottomn = offy + 2 * (CURSOR_PADDING / (GLfloat) SCREEN_HEIGHT);
	GLfloat bottoms = offy - 2 * ((CURSOR_MARGIN) / (GLfloat) SCREEN_HEIGHT);
	GLfloat leftw = offx + 2 * (CURSOR_MARGIN / (GLfloat) SCREEN_WIDTH);
	GLfloat lefte = offx - 2 * ((CURSOR_PADDING) / (GLfloat) SCREEN_WIDTH);
	offx += (GLfloat) 2 * (BLOCK_WIDTH) / (GLfloat) SCREEN_WIDTH;
	GLfloat midw = offx + 2 * (CURSOR_PADDING / (GLfloat) SCREEN_WIDTH);
	GLfloat mide = offx - 2 * ((CURSOR_PADDING) / (GLfloat) SCREEN_WIDTH);
	offx += (GLfloat) 2 * (BLOCK_WIDTH) / (GLfloat) SCREEN_WIDTH;
	GLfloat rightw = offx + 2 * (CURSOR_PADDING / (GLfloat) SCREEN_WIDTH);
	GLfloat righte = offx - 2 * ((CURSOR_MARGIN) / (GLfloat) SCREEN_WIDTH);
	if DEBUGIT_GAMEDRAW Root::logfile << "Game::drawcurs. " << offy << " " << topn << " " << tops << " " << bottomn <<
		" " << bottoms << "  " << offx << " " << leftw << " " << lefte << " " << 
		midw << " " << mide << " " << rightw << " " << righte << std::endl;
	
	// Top bar
	glBindTexture (GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f(leftw, topn);
		glVertex2f(leftw, tops);
		glVertex2f(righte, tops);
		glVertex2f(righte, topn);
	glEnd();
	// Bottom bar
	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f(leftw, bottomn);
		glVertex2f(leftw, bottoms);
		glVertex2f(righte, bottoms);
		glVertex2f(righte, bottomn);
	glEnd();
	// Left bar
	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex2f(leftw, tops);
		glVertex2f(lefte, tops);
		glVertex2f(lefte, bottomn);
		glVertex2f(leftw, bottomn);
	glEnd();
	// Mid bar
	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex2f(midw, tops);
		glVertex2f(mide, tops);
		glVertex2f(mide, bottomn);
		glVertex2f(midw, bottomn);
	glEnd();
	// Right bar
	glBegin(GL_QUADS);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glVertex2f(rightw, tops);
		glVertex2f(righte, tops);
		glVertex2f(righte, bottomn);
		glVertex2f(rightw, bottomn);
	glEnd();
}

void Field::draw(GLfloat offx, GLfloat offy, GLfloat extray_draw, GLfloat extray_tex) {
	int blockx = 0;
	int blocky = 0;
	GLfloat blockw = (GLfloat) 2*BLOCK_WIDTH / SCREEN_WIDTH;
	GLfloat blockh = (GLfloat) 2*BLOCK_HEIGHT / SCREEN_HEIGHT;
	if DEBUGIT_FIELDDRAW Root::logfile << "Field::draw	 Drawing field " << this->parentgame->id << "! " << offx << " " << offy << " " << extray_draw << " " << extray_tex << std::endl;
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

void Block::draw(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat extray_draw, GLfloat extray_tex) {
	if (this->face == BLOCKFACE_NOWT || this->face == BLOCKFACE_GARBAGE) return;
	if DEBUGIT_BLOCKDRAW Root::logfile << "Block::draw	 ---- " << x << " " << y << " " << w << " " << h << std::endl;
	int face = 0;
	if (this->facestate == FACESTATE_NORMAL) face += IMGMEM_OFFSET0;
	else if (this->facestate == FACESTATE_BLINK) face += IMGMEM_OFFSETA;
	else if (this->facestate == FACESTATE_CLEAR) face += IMGMEM_OFFSETB;
	else face = 0;
	face += IMGMEM__FRAMES * this->face;
	if DEBUGIT_BLOCKDRAW Root::logfile << "Block::draw	 Face = " << face << std::endl;
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

void SwapStack::draw(float offx, float offy) {
	for (int i = 0; i < this->stacklen; i++) {
		if (stack[i]) {
			if ((*stack[i]).draw(offx, offy)) { // If they returned 1, they're done for.
				delete stack[i];
				stack[i] = 0;
			}
		}
	}
}

int Swap::draw(float offx, float offy) {
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

int Popper::draw(float offx, float offy) {
	int facestate = FACESTATE_NORMAL;
	int startat = 0;
	this->framecount++;
	if (this->nowpopping == -2) {
		if ((this->framecount / 2) % 2) {
			facestate = FACESTATE_BLINK;
		}
		if (this->framecount >= TIME_POPSTACK_BEFORE) {
			if DEBUGIT_POPPERDRAW Root::logfile << "Popper::draw	Done blinking, ready to almost pop" << std::endl;
			this->nowpopping = -1;
			this->framecount = 0;
		}
	} else if (this->nowpopping == -1) {
		facestate = FACESTATE_CLEAR;
		if (this->framecount >= TIME_POPSTACK_THEN) {
			if DEBUGIT_POPPERDRAW Root::logfile << "Popper::draw	We're gonna pop!" << std::endl;
			this->nowpopping = 0;
			this->framecount = 0;
		}
	} else {
		facestate = FACESTATE_CLEAR;
		startat = this->nowpopping;
		if (this->framecount >= TIME_POPSTACK_EACH) {
			if DEBUGIT_POPPERDRAW Root::logfile << "Popper::draw	*pop* " << this->stackx[this->nowpopping] << "," << this->stacky[this->nowpopping] << std::endl;
			this->nowpopping++;
			this->framecount = 0;
		}
	}
	if (this->nowpopping >= this->stacklen) {
		if DEBUGIT_POPPERDRAW Root::logfile << "Popper::draw	Oh god we're done for!" << std::endl;
		return 1;
	}
	if (this->used != this->stacklen) {
		Root::logfile << "Popper::draw	-=-WARNING-=- We didn't get all our blocks yet! I don't know how this happened, but we only got " << this->used << "/" << this->stacklen << " and we're going LIVE NOW! D: Embrace yourselves!" << std::endl;
	}
	for (int i = startat; i < this->stacklen; i++) {
		GLfloat blockoffx = offx + 2*((GLfloat) (BLOCK_WIDTH * this->stackx[i]) / (GLfloat) SCREEN_WIDTH);
		GLfloat blockoffy = offy + 2*((GLfloat) (BLOCK_HEIGHT * (this->parentfield->getheight() - this->stacky[i]-1)) / (GLfloat) SCREEN_HEIGHT);
		stack[i]->facestate = facestate;
		stack[i]->draw(blockoffx, blockoffy, (GLfloat) 2*BLOCK_WIDTH / SCREEN_WIDTH, (GLfloat) 2*BLOCK_HEIGHT / SCREEN_HEIGHT, 0, 0);
	}
	return 0;
}

int Block::swapable() {
	if (this->face == BLOCKFACE_NOWT || this->face == BLOCKFACE_VS || !(this->state == BLOCKSTATE_STILL || this->state == BLOCKSTATE_SWITCHING || this->state == BLOCKSTATE_WILLFALL)) {
		return 0;
	}
	return 1;
}

int Field::swapable(int x, int y) {
	int w = getwidth();
	int h = getheight();
	int idx = y * w + x;
	if (!blocks[idx]) {
		if DEBUGIT_FIELDSWAPABLE Root::logfile << "Field::swapable " << x << "," << y << " doesn't exist!" << std::endl;
		if (y > 0) {
			int above = idx-w;
			if (blocks[above] && blocks[above]->state != BLOCKSTATE_FALLING) {
				if DEBUGIT_FIELDSWAPABLE Root::logfile << "Field::swapable " << x << "," << y << " The block above us exists, so we can't be switched!" << std::endl;
				return 0;
			}
		}
		if DEBUGIT_FIELDSWAPABLE Root::logfile << "Field::swapable " << x << "," << y << " ...but the aboves are clear so that doesn't matter!" << std::endl;
		return 1;
	} else {
		if (y+1 < h) {
			int below = idx+w;
			if (!blocks[below] || blocks[below]->state == BLOCKSTATE_SHH || blocks[below]->face == BLOCKFACE_NOWT) {
				if DEBUGIT_FIELDSWAPABLE Root::logfile << "Field::swapable " << x << "," << y << " The block under us doesn't exist, so we can't be switched!" << std::endl;
				return 0;
			}
		}
		if DEBUGIT_FIELDSWAPABLE Root::logfile << "Field::swapable " << x << "," << y << " We'll let the local block decide." << std::endl;
		return blocks[idx]->swapable();
	}
}

void Field::swap(int x1, int y1, int x2, int y2) {
	if DEBUGIT_FIELDSWAP Root::logfile << "Field::swap	 " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	int w = getwidth();
	int idx1 = y1*w+x1;
	int idx2 = y2*w+x2;
	Block *Block1 = this->blocks[idx1];
	Block *Block2 = this->blocks[idx2];
	if ((!Block1 && !Block2) || !(swapable(x1,y1) && swapable(x2,y2))) return;
	if (!Block1 && Block2) {
		if DEBUGIT_FIELDSWAP Root::logfile << "Field::swap	 We're moving a block from left to right, right space is currently empty." << std::endl;
		this->swapStack->swap(Block2, x2,y2,x1,y1, 1, this);
	} else if (Block1 && !Block2) {
		if DEBUGIT_FIELDSWAP Root::logfile << "Field::swap	 We're moving a block from right to left, left space is currently empty." << std::endl;
		this->swapStack->swap(Block1, x1,y1,x2,y2, 1, this);
	} else if (Block1 && Block2) {
		if DEBUGIT_FIELDSWAP Root::logfile << "Field::swap	 We're switching two blocks." << std::endl;
		this->swapStack->swap(Block1, x1,y1,x2,y2, 0, this);
		this->swapStack->swap(Block2, x2,y2,x1,y1, 1, this);
	} else {
		Root::logfile << "Field::swap	 -=-WARNING-=- We got in the else-part!" << std::endl;
	}
}

void Field::rawswap(int x1, int y1, int x2, int y2) {
	int i = 0;
	if (y2 != y1) {
		i++; // BREAK
	}
	if DEBUGIT_FIELDRAWSWAP Root::logfile << "Field::rawswap  " << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;
	int w = this->getwidth();
	int idx1 = y1*w+x1;
	int idx2 = y2*w+x2;
	Block *tempblock;
	if DEBUGIT_FIELDRAWSWAP Root::logfile << "Field::rawswap  -- " << idx1 << " " << this->blocks[idx1] << " <-> " << idx2 << " " << this->blocks[idx2] << std::endl;
	tempblock = this->blocks[idx1];
	this->blocks[idx1] = this->blocks[idx2];
	this->blocks[idx2] = tempblock;
	if DEBUGIT_FIELDRAWSWAP Root::logfile << "Field::rawswap  ++ " << idx1 << " " << this->blocks[idx1] << " <-> " << idx2 << " " << this->blocks[idx2] << std::endl;
	//if DEBUGIT_FIELDRAWSWAP Root::logfile << "Field::rawswap  No crash!" << std::endl;
}

void SwapStack::swap(Block *block, int fromx, int fromy, int tox, int toy, byte switchblock, Field *parent) {
	if (!block) {
		if DEBUGIT_SWAPSTACKSWAP Root::logfile << "SwapStack::swap Oh my, nullpointered block." << std::endl;
		return;
	}
	if (!block->swapable()) {
		if DEBUGIT_SWAPSTACKSWAP Root::logfile << "SwapStack::swap Oh my, block not swap(p)able." << std::endl;
		return;
	}
	block->state = BLOCKSTATE_SWITCHING;
	if (DEBUGIT_SWAPSTACKSWAP || DEBUGIT_SWAPSTACKSWAP_MEMORY) Root::logfile << "SwapStack::swap Starting." << std::endl;
	int idx = -1;
	for (int i = 0; i < stacklen; i++) {
		if (stack[i]) continue;
		if DEBUGIT_SWAPSTACKSWAP_MEMORY Root::logfile << "SwapStack::swap -- We got an unused swapstack index, " << i << ". We'll use that." << std::endl;
		idx = i;
		break;
	}
	if (idx < 0) {
		if (stacklen >= MAXSWAPS) {
			if DEBUGIT_SWAPSTACKSWAP_MEMORY Root::logfile << "SwapStack::swap canceled! oh my, we ran out of swaps." << std::endl;
			return;
		}
		stacklen++;
		idx = stacklen-1;
	}
	if (idx < 0) {
		if DEBUGIT_SWAPSTACKSWAP_MEMORY Root::logfile << "SwapStack::swap -- Hmm! idx is still lower than 0 even though we went through making a new stack and all. " << idx << std::endl;
	}
	if DEBUGIT_SWAPSTACKSWAP_MEMORY Root::logfile << "SwapStack::swap -- stack[idx = " << idx << "] = new Swap(block = " << block << ", fromx,y,tox,y = " << fromx << "," << fromy << "," << tox << "," << toy << ")" << std::endl;
	stack[idx] = new Swap(block, fromx, fromy, tox, toy, switchblock, parent);
	if (DEBUGIT_SWAPSTACKSWAP || DEBUGIT_SWAPSTACKSWAP_MEMORY) Root::logfile << "SwapStack::swap ending." << std::endl;
}

Swap::Swap(Block *block_, int fromx_, int fromy_, int tox_, int toy_, byte switchblock_, class Field *parentfield_) {
	this->block = block_;
	this->fromx = fromx_;
	this->fromy = fromy_;
	this->tox = tox_;
	this->toy = toy_;
	this->switchblock = switchblock_;
	this->parentfield = parentfield_;
	this->frame = 0;
	if (this->switchblock) {
		if DEBUGIT_SWAPCONSTRUCT Root::logfile << "Swap::Swap	  Rawswapping (" << (int) this->switchblock << ") " << fromx << "," << fromy << " " << tox << "," << toy << std::endl;
		this->parentfield->rawswap(fromx,fromy,tox,toy);
	} else {
		if DEBUGIT_SWAPCONSTRUCT Root::logfile << "Swap::Swap	  Not rawswapping (" << (int) this->switchblock << ") " << fromx << "," << fromy << " " << tox << "," << toy << std::endl;
	}
}

void Popper::addblock(Block *block, int x, int y) {
	int idx = this->used++;
	if (idx > this->stacklen) return;
	this->stack[idx] = block;
	this->stackx[idx] = x;
	this->stacky[idx] = y;
	block->state = BLOCKSTATE_POPPING;
	if (!ischain) {
		if (block->ischain) ischain = 1;
		if (DEBUGIT_POPPERADD && ischain) Root::logfile << "Swap::addblock  Oh my, added a chain block." << std::endl;
	}
}

Popper::Popper(int count, class Field *parentfield_) {
	stacklen = count;
	for (int i = 0; i < count; i++) {
		stack[i] = 0;
		stackx[i] = 0;
		stacky[i] = 0;
	}
	used = 0;
	framecount = 0;
	nowpopping = -2;
	parentfield = parentfield_;
	ischain = 0;
}

void Popper::clean() {
	for (int i = 0; i < stacklen; i++) {
		if (this->stack[i]) {
			int x = this->stackx[i];
			int y = this->stacky[i];
			this->parentfield->deleteblock(x, y);
			this->stack[i] = 0;
			while (y-- && this->parentfield->blockfallchain(x,y)) {}
		}
	}
}

int Field::getwidth() {
	return this->width;
}

int Field::getheight() {
	return this->height;
}

short Field::getcolors() {
	return this->colors;
}

byte Field::gethasvs() {
	return this->hasvs;
}

bool Field::blockis(int x, int y) {
	int idx = y*getwidth()+x;
	if (this->blocks[idx]) return true;
	return false;
}

int Field::blockface(int x, int y) {
	int idx = y*getwidth()+x;
	if (this->blocks[idx]) return this->blocks[y*getwidth()+x]->face;
	return BLOCKFACE_NOWT;
}

int Field::blockstate(int x, int y) {
	int idx = y*getwidth()+x;
	if (this->blocks[idx]) return this->blocks[y*getwidth()+x]->state;
	return BLOCKSTATE_SHH;
}

byte Field::blockischain(int x, int y) {
	int idx = y*getwidth()+x;
	if (this->blocks[idx]) return this->blocks[y*getwidth()+x]->ischain ? 1 : 0;
	return 0;
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
				Root::logfile << "Field::newfield Hmm! Field->blocks[" << newidx << "] already set!" << std::endl;
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
			int color = Root::rnd(1, colors-(blockleft?1:0)-(blockbeneath?1:0));
			if DEBUGIT_NEWFIELD {
				Root::logfile << "Field::newfield " << i << "," << j << " " << color << " ";
				if (blockbeneath || blockleft) {
					Root::logfile << "(";
					if (blockleft) {
						Root::logfile << "left " << blockleft->face;
						if (blockbeneath) Root::logfile << ", ";
					}
					if (blockbeneath) Root::logfile << "under " << blockbeneath->face;
					Root::logfile << ") ";
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
			if DEBUGIT_NEWFIELD Root::logfile << "Field::newfield " << color << std::endl;
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
		int newface = Root::rnd(1, count);
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
		if DEBUGIT_NEWLINE Root::logfile << "Field::newline  Face " << i << ": " << newface << std::endl;
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

void Root::init() {
	Root::logfile.open("log");
	Root::logfile << WINNAME << " " << VERSION << " log" << std::endl << "Rand_max: " << RAND_MAX << std::endl;
	Root::state = STATE_TITLE;
	Root::CLOCKS_PER_FRAME = (CLOCKS_PER_SEC / 80);
}

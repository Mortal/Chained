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
	
	if DEBUGIT_DRAWBACKGROUND logfile << "Game::drawbg.   " << this->id << " " << n << " " << s << " " << w << " " << e << std::endl;
	if DEBUGIT_DRAWBACKGROUND logfile << "Game::drawbg.   " << this->id << " " << n*SCREEN_HEIGHT << " " << s*SCREEN_HEIGHT << " " << w*SCREEN_WIDTH << " " << e*SCREEN_WIDTH << std::endl;
	if DEBUGIT_DRAWBACKGROUND logfile << "Game::drawbg.   " << this->id << " " << (n-s)*SCREEN_HEIGHT << " " << (e-w)*SCREEN_WIDTH << std::endl;
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
	if DEBUGIT_GAMEDRAW logfile << "Game::drawcurs. " << offy << " " << topn << " " << tops << " " << bottomn <<
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
			if DEBUGIT_POPPERDRAW logfile << "Popper::draw	Done blinking, ready to almost pop" << std::endl;
			this->nowpopping = -1;
			this->framecount = 0;
		}
	} else if (this->nowpopping == -1) {
		facestate = FACESTATE_CLEAR;
		if (this->framecount >= TIME_POPSTACK_THEN) {
			if DEBUGIT_POPPERDRAW logfile << "Popper::draw	We're gonna pop!" << std::endl;
			this->nowpopping = 0;
			this->framecount = 0;
		}
	} else {
		facestate = FACESTATE_CLEAR;
		startat = this->nowpopping;
		if (this->framecount >= TIME_POPSTACK_EACH) {
			if DEBUGIT_POPPERDRAW logfile << "Popper::draw	*pop* " << this->stackx[this->nowpopping] << "," << this->stacky[this->nowpopping] << std::endl;
			this->nowpopping++;
			this->framecount = 0;
		}
	}
	if (this->nowpopping >= this->stacklen) {
		if DEBUGIT_POPPERDRAW logfile << "Popper::draw	Oh god we're done for!" << std::endl;
		return 1;
	}
	if (this->used != this->stacklen) {
		logfile << "Popper::draw	-=-WARNING-=- We didn't get all our blocks yet! I don't know how this happened, but we only got " << this->used << "/" << this->stacklen << " and we're going LIVE NOW! D: Embrace yourselves!" << std::endl;
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
		if DEBUGIT_FIELDSWAPABLE logfile << "Field::swapable " << x << "," << y << " doesn't exist!" << std::endl;
		if (y > 0) {
			int above = idx-w;
			if (blocks[above] && blocks[above]->state != BLOCKSTATE_FALLING) {
				if DEBUGIT_FIELDSWAPABLE logfile << "Field::swapable " << x << "," << y << " The block above us exists, so we can't be switched!" << std::endl;
				return 0;
			}
		}
		if DEBUGIT_FIELDSWAPABLE logfile << "Field::swapable " << x << "," << y << " ...but the aboves are clear so that doesn't matter!" << std::endl;
		return 1;
	} else {
		if (y+1 < h) {
			int below = idx+w;
			if (!blocks[below] || blocks[below]->state == BLOCKSTATE_SHH || blocks[below]->face == BLOCKFACE_NOWT) {
				if DEBUGIT_FIELDSWAPABLE logfile << "Field::swapable " << x << "," << y << " The block under us doesn't exist, so we can't be switched!" << std::endl;
				return 0;
			}
		}
		if DEBUGIT_FIELDSWAPABLE logfile << "Field::swapable " << x << "," << y << " We'll let the local block decide." << std::endl;
		return blocks[idx]->swapable();
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

void SwapStack::swap(Block *block, int fromx, int fromy, int tox, int toy, byte switchblock, Field *parent) {
	if (!block) {
		if DEBUGIT_SWAPSTACKSWAP logfile << "SwapStack::swap Oh my, nullpointered block." << std::endl;
		return;
	}
	if (!block->swapable()) {
		if DEBUGIT_SWAPSTACKSWAP logfile << "SwapStack::swap Oh my, block not swap(p)able." << std::endl;
		return;
	}
	block->state = BLOCKSTATE_SWITCHING;
	if (DEBUGIT_SWAPSTACKSWAP || DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap Starting." << std::endl;
	int idx = -1;
	for (int i = 0; i < stacklen; i++) {
		if (stack[i]) continue;
		if DEBUGIT_SWAPSTACKSWAP_MEMORY logfile << "SwapStack::swap -- We got an unused swapstack index, " << i << ". We'll use that." << std::endl;
		idx = i;
		break;
	}
	if (idx < 0) {
		if (stacklen >= MAXSWAPS) {
			if DEBUGIT_SWAPSTACKSWAP_MEMORY logfile << "SwapStack::swap canceled! oh my, we ran out of swaps." << std::endl;
			return;
		}
		stacklen++;
		idx = stacklen-1;
	}
	if (idx < 0) {
		if DEBUGIT_SWAPSTACKSWAP_MEMORY logfile << "SwapStack::swap -- Hmm! idx is still lower than 0 even though we went through making a new stack and all. " << idx << std::endl;
	}
	if DEBUGIT_SWAPSTACKSWAP_MEMORY logfile << "SwapStack::swap -- stack[idx = " << idx << "] = new Swap(block = " << block << ", fromx,y,tox,y = " << fromx << "," << fromy << "," << tox << "," << toy << ")" << std::endl;
	stack[idx] = new Swap(block, fromx, fromy, tox, toy, switchblock, parent);
	if (DEBUGIT_SWAPSTACKSWAP || DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap ending." << std::endl;
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
		if DEBUGIT_SWAPCONSTRUCT logfile << "Swap::Swap	  Rawswapping (" << (int) this->switchblock << ") " << fromx << "," << fromy << " " << tox << "," << toy << std::endl;
		this->parentfield->rawswap(fromx,fromy,tox,toy);
	} else {
		if DEBUGIT_SWAPCONSTRUCT logfile << "Swap::Swap	  Not rawswapping (" << (int) this->switchblock << ") " << fromx << "," << fromy << " " << tox << "," << toy << std::endl;
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
		if (DEBUGIT_POPPERADD && ischain) logfile << "Swap::addblock  Oh my, added a chain block." << std::endl;
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

/* vim:set ts=4 sw=4 sts=4 noet: */
// Mortal's Tetris Attack clone, begun May 5th 2007.

#include "winmain.h"
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

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
bool paused = false;
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
std::ofstream logfile;

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
			if (DEBUGIT_KEYS) logfile << "key	  " << ((char) (key & 0xFF)) << " direction " << ((int) (dir)) << std::endl;
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
						if (paused) paused = false;
						else {
							goframe = 0;
							paused = true;
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
				if (DEBUGIT_ROOTDRAW) logfile << "draw	  " << x << "," << y << " ";
				glTexCoord2f (0.0f, 0.0f);
				glVertex2f (x, y); // NW
				float oldy = y;
				y = 2*(0.5f - (float) 2*(float) BLOCK_HEIGHT/(float) SCREEN_HEIGHT);
				if (DEBUGIT_ROOTDRAW) logfile << "draw	  " << x << "," << y << " ";
				glTexCoord2f (0.0f, 1.0f);
				glVertex2f (x, y); // NE
				x = 2*(-0.5f + ((2.0f+(2.0f*(j-1.0f)))*(float) BLOCK_WIDTH)/(float) SCREEN_WIDTH);
				if (DEBUGIT_ROOTDRAW) logfile << "draw	  " << x << "," << y << " ";
				glTexCoord2f (1.0f, 1.0f);
				glVertex2f (x, y); // SE
				y = oldy;
				if (DEBUGIT_ROOTDRAW) logfile << "draw	  " << x << "," << y << std::endl;
				glTexCoord2f (1.0f, 0.0f);
				glVertex2f (x, y); // SW
				glEnd ();
			}
		break;
	}
}

void init() {
	logfile.open("log");
	logfile << WINNAME << " " << VERSION << " log" << std::endl << "Rand_max: " << RAND_MAX << std::endl;
	state = STATE_TITLE;
	CLOCKS_PER_FRAME = (CLOCKS_PER_SEC / 80);
}

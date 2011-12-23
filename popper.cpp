#include "popper.h"
#include "field.h"

int Popper::draw(float offx, float offy) {
	int facestate = FACESTATE_NORMAL;
	int startat = 0;
	this->framecount++;
	if (this->nowpopping == -2) {
		if ((this->framecount / 2) % 2) {
			facestate = FACESTATE_BLINK;
		}
		if (this->framecount >= TIME_POPSTACK_BEFORE) {
			if (DEBUGIT_POPPERDRAW) logfile << "Popper::draw	Done blinking, ready to almost pop" << std::endl;
			this->nowpopping = -1;
			this->framecount = 0;
		}
	} else if (this->nowpopping == -1) {
		facestate = FACESTATE_CLEAR;
		if (this->framecount >= TIME_POPSTACK_THEN) {
			if (DEBUGIT_POPPERDRAW) logfile << "Popper::draw	We're gonna pop!" << std::endl;
			this->nowpopping = 0;
			this->framecount = 0;
		}
	} else {
		facestate = FACESTATE_CLEAR;
		startat = this->nowpopping;
		if (this->framecount >= TIME_POPSTACK_EACH) {
			if (DEBUGIT_POPPERDRAW) logfile << "Popper::draw	*pop* " << this->stackx[this->nowpopping] << "," << this->stacky[this->nowpopping] << std::endl;
			this->nowpopping++;
			this->framecount = 0;
		}
	}
	if (this->nowpopping >= this->stacklen) {
		if (DEBUGIT_POPPERDRAW) logfile << "Popper::draw	Oh god we're done for!" << std::endl;
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

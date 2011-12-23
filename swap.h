/* vim:set ts=4 sw=4 sts=4 noet: */
#include "constants.h"

class Swap {
	public:
		class Block *block;
		byte switchblock;
		class Field *parentfield;
		int fromx,fromy,tox,toy;
		Swap(Block *block_, int fromx_, int fromy_, int tox_, int toy_, byte switchblock_, class Field *parentfield);
		int draw(float offx, float offy);
	private:
		int frame;
};

class SwapStack {
	public:
		void draw(float offx, float offy);
		void swap(Block *block, int fromx, int fromy, int tox, int toy, byte switchblock, class Field *parent);
		SwapStack() {this->stacklen = 0;}
		int swapCount();
		
	private:
		class Swap *stack[MAXSWAPS];
		int stacklen;
};

/* vim:set ts=4 sw=4 sts=4 noet: */
#include "swap.h"

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

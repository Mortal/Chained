/* vim:set ts=4 sw=4 sts=4 noet: */
#include "swap.h"

class SwapStack {
	public:
		void draw(float offx, float offy) {
			for (int i = 0; i < this->stacklen; i++) {
				if (stack[i]) {
					if ((*stack[i]).draw(offx, offy)) { // If they returned 1, they're done for.
						delete stack[i];
						stack[i] = 0;
					}
				}
			}
		}
		void swap(Block *block, int fromx, int fromy, int tox, int toy, byte switchblock, class Field *parent);
		SwapStack() {this->stacklen = 0;}
		inline int swapCount() {
			int count = 0;
			for (int i = 0; i < stacklen; i++) {
				if (stack[i]) {
					if DEBUGIT_SWAPSTACKSWAPCOUNT logfile << "SwSt::swCount   stack[" << i << "] is in use: " << stack[i] << std::endl;
					count++;
				}
			}
			return count;
		}
		
	private:
		class Swap *stack[MAXSWAPS];
		int stacklen;
};

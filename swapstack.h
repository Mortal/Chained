/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_SWAPSTACK
#define CHAINED_SWAPSTACK
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
		void swap(Block *block, int fromx, int fromy, int tox, int toy, byte switchblock, class Field *parent) {
			if (!block) {
				if (DEBUGIT_SWAPSTACKSWAP) logfile << "SwapStack::swap Oh my, nullpointered block." << std::endl;
				return;
			}
			if (!block->swapable()) {
				if (DEBUGIT_SWAPSTACKSWAP) logfile << "SwapStack::swap Oh my, block not swap(p)able." << std::endl;
				return;
			}
			block->state = BLOCKSTATE_SWITCHING;
			if (DEBUGIT_SWAPSTACKSWAP || DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap Starting." << std::endl;
			int idx = -1;
			for (int i = 0; i < stacklen; i++) {
				if (stack[i]) continue;
				if (DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap -- We got an unused swapstack index, " << i << ". We'll use that." << std::endl;
				idx = i;
				break;
			}
			if (idx < 0) {
				if (stacklen >= MAXSWAPS) {
					if (DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap canceled! oh my, we ran out of swaps." << std::endl;
					return;
				}
				stacklen++;
				idx = stacklen-1;
			}
			if (idx < 0) {
				if (DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap -- Hmm! idx is still lower than 0 even though we went through making a new stack and all. " << idx << std::endl;
			}
			if (DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap -- stack[idx = " << idx << "] = new Swap(block = " << block << ", fromx,y,tox,y = " << fromx << "," << fromy << "," << tox << "," << toy << ")" << std::endl;
			stack[idx] = new Swap(block, fromx, fromy, tox, toy, switchblock, parent);
			if (DEBUGIT_SWAPSTACKSWAP || DEBUGIT_SWAPSTACKSWAP_MEMORY) logfile << "SwapStack::swap ending." << std::endl;
		}
		SwapStack() {this->stacklen = 0;}
		inline int swapCount() {
			int count = 0;
			for (int i = 0; i < stacklen; i++) {
				if (stack[i]) {
					if (DEBUGIT_SWAPSTACKSWAPCOUNT) logfile << "SwSt::swCount   stack[" << i << "] is in use: " << stack[i] << std::endl;
					count++;
				}
			}
			return count;
		}
		
	private:
		class Swap *stack[MAXSWAPS];
		int stacklen;
};
#endif

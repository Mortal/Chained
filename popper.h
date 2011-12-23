/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_POPPER
#define CHAINED_POPPER
#include "constants.h"
#include "block.h"

class Popper {
	public:
		int draw(float offx, float offy);
		void addblock(class Block *block, int x, int y) {
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
		Popper(int count, class Field *parentfield_) {
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
		void clean();
		
	private:
		class Field *parentfield;
		class Block *stack[MAXPOPPERS];
		int stackx[MAXPOPPERS];
		int stacky[MAXPOPPERS];
		int stacklen;
		int used;
		int framecount;
		int nowpopping;
		int ischain;
};
#endif

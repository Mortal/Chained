#include "constants.h"

class Popper {
	public:
		int draw(float offx, float offy);
		void addblock(class Block *block, int x, int y);
		Popper(int count, class Field *parentfield_);
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

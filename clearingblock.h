/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_CLEARINGBLOCK
#define CHAINED_CLEARINGBLOCK
class ClearingBlock {
	public:
		int face;
		int frame;
		int offx;
		int offy;
		ClearingBlock(int face, int offx, int offy) {
			this->face = face;
			this->offx = offx;
			this->offy = offy;
			this->frame = 0;
		}
};
#endif

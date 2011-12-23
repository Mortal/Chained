/* vim:set ts=4 sw=4 sts=4 noet: */
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

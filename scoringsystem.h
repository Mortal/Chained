/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_SCORINGSYSTEM
#define CHAINED_SCORINGSYSTEM

class ScoringSystem {
	public:
		int combo(int count);
		int chain(int count);
		int chainbonus(int count);
		int blockpop();
		int stack();
};

class SS_Classic_infchain : public ScoringSystem {
	public:
		int combo(int count);
		int chain(int count);
		int chainbonus(int count);
		int blockpop();
		int stack();
};

class SS_Classic_limitchain : public SS_Classic_infchain {
	public:
		int combo(int count);
		int chain(int count);
		int chainbonus(int count);
		int blockpop();
		int stack();
};
#endif

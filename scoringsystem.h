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

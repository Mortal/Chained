#include "scoringsystem.h"

// Classic SNESTA/PDP scoring, except >x13 allowed.
int SS_Classic_infchain::combo(int count) {
	if (count <= 3) return 0;
	if (count <= 5) return (count-2)*10;
	if (count <= 9) return (count-1)*10;
	if (count <= 10) return count*10;
	return count*30-190;
}

int SS_Classic_infchain::chain(int count) {
	if (count <= 1) return 0;
	if (count <= 3) return count*30+20;
	if (count <= 5) return (count-3)*150;
	if (count <= 7) return (count-2)*100;
	if (count <= 12) return count*200-900;
	return count*300-2100;
}

int SS_Classic_infchain::chainbonus(int count) {
	return chain(count);
}

int SS_Classic_infchain::blockpop() {
	return 10;
}

int SS_Classic_infchain::stack() {
	return 1;
}

// Classic SNESTA/PDP scoring, >x13 gives nothing.
int SS_Classic_limitchain::combo(int count) {
	return SS_Classic_infchain::combo(count);
}

int SS_Classic_limitchain::chain(int count) {
	if (count > 13) return 0;
	return SS_Classic_infchain::chain(count);
}

int SS_Classic_limitchain::chainbonus(int count) {
	return chain(count);
}

int SS_Classic_limitchain::blockpop() {
	return 10;
}

int SS_Classic_limitchain::stack() {
	return 1;
}

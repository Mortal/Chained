/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_CONSTANTS
#define CHAINED_CONSTANTS
const char * UNIXNAME = "chained";
const char * WINNAME = "Chained";
const char * VERSION = "0.0.8";
const int VERSIONMATH = (8);
// aaaiiirrr (major minor revision)
const int FIELD_WIDTH = (6);
const int FIELD_HEIGHT = (12);
const int FIELD_COLORS = (5);
const int BLOCK_SRCWIDTH = (16);
const int BLOCK_SRCHEIGHT = (16);
const int BLOCK_SCALE = (2);
const int BLOCK_WIDTH = (BLOCK_SCALE*BLOCK_SRCWIDTH);
const int BLOCK_HEIGHT = (BLOCK_SCALE*BLOCK_SRCHEIGHT);
const int BLOCK_MAXFRAMES = (100);
const int CURSOR_MARGIN = (2);
const int CURSOR_PADDING = (1);
const int SCREEN_WIDTH = (FIELD_WIDTH*BLOCK_WIDTH);
const int SCREEN_HEIGHT = (FIELD_HEIGHT*BLOCK_HEIGHT);

/* The number of frames to hold a key before it repeats */
const int KEYTOHOLD = (15);

// Debugging

	// Misc
	const int DEBUGIT_KEYS = (0);
	const int DEBUGIT_KEYS_ALL = (0);
	const int DEBUGIT_KEYS_UNCAUGHT = (0);
	const int DEBUGIT_NEWFIELD = (0);
	const int DEBUGIT_NEWLINE = (0);
	const int DEBUGIT_FIELDRAISESTACK = (0);
	
	// Swapping
	const int DEBUGIT_FIELDSWAP = (0);
	const int DEBUGIT_FIELDSWAPABLE = (0);
	const int DEBUGIT_SWAPSTACKSWAP = (0);
	const int DEBUGIT_SWAPSTACKSWAP_MEMORY = (0);
	const int DEBUGIT_FIELDRAWSWAP = (0);
	const int DEBUGIT_SWAPSTACKSWAPCOUNT = (0);
	
	const int DEBUGIT_SWAPCONSTRUCT = (0);
	
	const int DEBUGIT_FIELDCHECKBLOCKS = (0);
	const int DEBUGIT_FIELDFALLBLOCKS = (0);
	const int DEBUGIT_FIELDPOPPERCOUNT = (0);
	
	const int DEBUGIT_POPPERADD = (0);
	
	// Drawing
	const int DEBUGIT_ROOTDRAW = (0);
	const int DEBUGIT_GAMEDRAW = (0);
	const int DEBUGIT_FIELDDRAW = (0);
	const int DEBUGIT_BLOCKDRAW = (0);
	const int DEBUGIT_SWAPDRAW = (0);
	const int DEBUGIT_POPPERDRAW = (0);
	const int DEBUGIT_DRAWBACKGROUND = (0);

// Timings (in frames)
const int TIME_SWITCH = (5);
const int TIME_POPSTACK_BEFORE = (44);
const int TIME_POPSTACK_THEN = (25);
const int TIME_POPSTACK_EACH = (9);
const int TIME_FALL_BEFORE = (18);
const int TIME_FALL_BEFORE_M = (8);
const int TIME_FALL_BEFORE_H = (5);

const int STACKOFFSET_MAX = (16);

const int MAXSWAPS = (100);
const int MAXPOPPERS = (100);

const int BLOCKFACE_NOWT = (0);
const int BLOCKFACE_RED = (1);
const int BLOCKFACE_YELLOW = (2);
const int BLOCKFACE_GREEN = (3);
const int BLOCKFACE_CYAN = (4);
const int BLOCKFACE_PURPLE = (5);
const int BLOCKFACE_BLUE = (6);
const int BLOCKFACE_VS = (7);
const int BLOCKFACE_GARBAGE = (8);
const int BLOCKFACE_EXAMPLE = (9);

const int FACESTATE_NORMAL = (0);
const int FACESTATE_BLINK = (1);
const int FACESTATE_CLEAR = (2);

//#define IMGMEM_REDBLOCK (1)
//#define IMGMEM_YELLOWBLOCK (2)
//#define IMGMEM_GREENBLOCK (3)
//#define IMGMEM_CYANBLOCK (4)
//#define IMGMEM_PURPLEBLOCK (5)
//#define IMGMEM_BLUEBLOCK (6)
//#define IMGMEM_VSBLOCK (7)
//#define IMGMEM_EXAMPLEBLOCK (9)
const int IMGMEM_OFFSET0 = (0);
const int IMGMEM_OFFSETA = (1500);
const int IMGMEM_OFFSETB = (3000);
const int IMGMEM__FRAMES = (100);

const int STATE_TITLE = (0);
const int STATE_MENU = (1);
const int STATE_GAME = (2);
const int STATE_DEBUG = (255);

const int GARBAGECOLOR_GRAY = (0);
const int GARBAGECOLOR_GREY = (0);
const int GARBAGECOLOR_RED = (1);
const int GARBAGECOLOR_BLUE = (2);

const int BLOCKSTATE_SHH = (0);
const int BLOCKSTATE_STILL = (1);
const int BLOCKSTATE_WILLFALL = (2);
const int BLOCKSTATE_FALLING = (3);
const int BLOCKSTATE_SWITCHING = (4);
const int BLOCKSTATE_POPPING = (5);
const int BLOCKSTATE_WAITING = (6);

const int GAMEKEY_LEFT = (0);
const int GAMEKEY_UP = (1);
const int GAMEKEY_RIGHT = (2);
const int GAMEKEY_DOWN = (3);
const int GAMEKEY_SWITCH = (4);
const int GAMEKEY_STACK = (5);

const int KEYTYPE_DOWN = (0);
const int KEYTYPE_UP = (1);
const int KEYTYPE_REPEAT = (2);

typedef unsigned char byte;
#endif

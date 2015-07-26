/* vim:set ts=4 sw=4 sts=4 noet: */
#ifndef CHAINED_CONSTANTS
#define CHAINED_CONSTANTS
static const char * UNIXNAME = "chained";
static const char * WINNAME = "Chained";
static const char * VERSION = "0.0.8";
static const int VERSIONMATH = (8);
// aaaiiirrr (major minor revision)
static const int FIELD_WIDTH = (6);
static const int FIELD_HEIGHT = (12);
static const int FIELD_COLORS = (5);
static const int BLOCK_SRCWIDTH = (16);
static const int BLOCK_SRCHEIGHT = (16);
static const int BLOCK_SCALE = (2);
static const int BLOCK_WIDTH = (BLOCK_SCALE*BLOCK_SRCWIDTH);
static const int BLOCK_HEIGHT = (BLOCK_SCALE*BLOCK_SRCHEIGHT);
static const int BLOCK_MAXFRAMES = (100);
static const int CURSOR_MARGIN = (2);
static const int CURSOR_PADDING = (1);
static const int SCREEN_WIDTH = (FIELD_WIDTH*BLOCK_WIDTH);
static const int SCREEN_HEIGHT = (FIELD_HEIGHT*BLOCK_HEIGHT);

/* The number of frames to hold a key before it repeats */
static const int KEYTOHOLD = (15);

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
static const int TIME_SWITCH = (5);
static const int TIME_POPSTACK_BEFORE = (44);
static const int TIME_POPSTACK_THEN = (25);
static const int TIME_POPSTACK_EACH = (9);
static const int TIME_FALL_BEFORE = (18);
static const int TIME_FALL_BEFORE_M = (8);
static const int TIME_FALL_BEFORE_H = (5);

static const int STACKOFFSET_MAX = (16);

static const int MAXSWAPS = (100);
static const int MAXPOPPERS = (100);

static const int BLOCKFACE_NOWT = (0);
static const int BLOCKFACE_RED = (1);
static const int BLOCKFACE_YELLOW = (2);
static const int BLOCKFACE_GREEN = (3);
static const int BLOCKFACE_CYAN = (4);
static const int BLOCKFACE_PURPLE = (5);
static const int BLOCKFACE_BLUE = (6);
static const int BLOCKFACE_VS = (7);
static const int BLOCKFACE_GARBAGE = (8);
static const int BLOCKFACE_EXAMPLE = (9);

static const int FACESTATE_NORMAL = (0);
static const int FACESTATE_BLINK = (1);
static const int FACESTATE_CLEAR = (2);

//#define IMGMEM_REDBLOCK (1)
//#define IMGMEM_YELLOWBLOCK (2)
//#define IMGMEM_GREENBLOCK (3)
//#define IMGMEM_CYANBLOCK (4)
//#define IMGMEM_PURPLEBLOCK (5)
//#define IMGMEM_BLUEBLOCK (6)
//#define IMGMEM_VSBLOCK (7)
//#define IMGMEM_EXAMPLEBLOCK (9)
// static const int IMGMEM_OFFSET0 = (0);
// static const int IMGMEM_OFFSETA = (1500);
// static const int IMGMEM_OFFSETB = (3000);
// static const int IMGMEM__FRAMES = (100);

static const int STATE_TITLE = (0);
static const int STATE_MENU = (1);
static const int STATE_GAME = (2);
static const int STATE_DEBUG = (255);

static const int GARBAGECOLOR_GRAY = (0);
static const int GARBAGECOLOR_GREY = (0);
static const int GARBAGECOLOR_RED = (1);
static const int GARBAGECOLOR_BLUE = (2);

static const int BLOCKSTATE_SHH = (0);
static const int BLOCKSTATE_STILL = (1);
static const int BLOCKSTATE_WILLFALL = (2);
static const int BLOCKSTATE_FALLING = (3);
static const int BLOCKSTATE_SWITCHING = (4);
static const int BLOCKSTATE_POPPING = (5);
static const int BLOCKSTATE_WAITING = (6);

static const int GAMEKEY_LEFT = (0);
static const int GAMEKEY_UP = (1);
static const int GAMEKEY_RIGHT = (2);
static const int GAMEKEY_DOWN = (3);
static const int GAMEKEY_SWITCH = (4);
static const int GAMEKEY_STACK = (5);

static const int KEYTYPE_DOWN = (0);
static const int KEYTYPE_UP = (1);
static const int KEYTYPE_REPEAT = (2);

typedef unsigned char byte;
#endif

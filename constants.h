#define UNIXNAME "chained"
#define WINNAME "Chained"
#define VERSION "0.0.8"
#define VERSIONMATH (8)
// aaaiiirrr (major minor revision)
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (500)
#define BLOCK_WIDTH (32)
#define BLOCK_HEIGHT (32)
#define BLOCK_SRCWIDTH (16)
#define BLOCK_SRCHEIGHT (16)
#define BLOCK_MAXFRAMES (100)
#define CURSOR_MARGIN (2)
#define CURSOR_PADDING (1)

/* The number of frames to hold a key before it repeats */
#define KEYTOHOLD (15)

//#define CLOCKS_PER_FRAME (CLOCKS_PER_SEC / 60)

// Debugging

    // Misc
    #define DEBUGIT_KEYS (0)
    #define DEBUGIT_KEYS_ALL (0)
    #define DEBUGIT_KEYS_UNCAUGHT (0)
    #define DEBUGIT_NEWFIELD (0)
    #define DEBUGIT_NEWLINE (0)
    #define DEBUGIT_FIELDRAISESTACK (0)
    
    // Swapping
    #define DEBUGIT_FIELDSWAP (0)
    #define DEBUGIT_FIELDSWAPABLE (0)
    #define DEBUGIT_SWAPSTACKSWAP (0)
    #define DEBUGIT_SWAPSTACKSWAP_MEMORY (0)
    #define DEBUGIT_FIELDRAWSWAP (0)
    #define DEBUGIT_SWAPSTACKSWAPCOUNT (0)
    
    #define DEBUGIT_SWAPCONSTRUCT (0)
    
    #define DEBUGIT_FIELDCHECKBLOCKS (0)
    #define DEBUGIT_FIELDFALLBLOCKS (0)
    #define DEBUGIT_FIELDPOPPERCOUNT (0)
    
    #define DEBUGIT_POPPERADD (0)
    
    // Drawing
    #define DEBUGIT_ROOTDRAW (0)
    #define DEBUGIT_GAMEDRAW (0)
    #define DEBUGIT_FIELDDRAW (0)
    #define DEBUGIT_BLOCKDRAW (0)
    #define DEBUGIT_SWAPDRAW (0)
    #define DEBUGIT_POPPERDRAW (0)
    #define DEBUGIT_DRAWBACKGROUND (0)

// Timings (in frames)
#define TIME_SWITCH (5)
#define TIME_POPSTACK_BEFORE (44)
#define TIME_POPSTACK_THEN (25)
#define TIME_POPSTACK_EACH (9)
#define TIME_FALL_BEFORE (10)
#define TIME_FALL_BEFORE_M (8)
#define TIME_FALL_BEFORE_H (5)

#define STACKOFFSET_MAX (16)

#define MAXSWAPS (100)
#define MAXPOPPERS (100)

#define BLOCKFACE_NOWT (0)
#define BLOCKFACE_RED (1)
#define BLOCKFACE_YELLOW (2)
#define BLOCKFACE_GREEN (3)
#define BLOCKFACE_CYAN (4)
#define BLOCKFACE_PURPLE (5)
#define BLOCKFACE_BLUE (6)
#define BLOCKFACE_VS (7)
#define BLOCKFACE_GARBAGE (8)
#define BLOCKFACE_EXAMPLE (9)

#define FACESTATE_NORMAL (0)
#define FACESTATE_BLINK (1)
#define FACESTATE_CLEAR (2)

//#define IMGMEM_REDBLOCK (1)
//#define IMGMEM_YELLOWBLOCK (2)
//#define IMGMEM_GREENBLOCK (3)
//#define IMGMEM_CYANBLOCK (4)
//#define IMGMEM_PURPLEBLOCK (5)
//#define IMGMEM_BLUEBLOCK (6)
//#define IMGMEM_VSBLOCK (7)
//#define IMGMEM_EXAMPLEBLOCK (9)
#define IMGMEM_OFFSET0 (0)
#define IMGMEM_OFFSETA (1500)
#define IMGMEM_OFFSETB (3000)
#define IMGMEM__FRAMES (100)

#define STATE_TITLE (0)
#define STATE_MENU (1)
#define STATE_GAME (2)
#define STATE_DEBUG (255)

#define GARBAGECOLOR_GRAY (0)
#define GARBAGECOLOR_GREY (0)
#define GARBAGECOLOR_RED (1)
#define GARBAGECOLOR_BLUE (2)

#define BLOCKSTATE_SHH (0)
#define BLOCKSTATE_STILL (1)
#define BLOCKSTATE_WILLFALL (2)
#define BLOCKSTATE_FALLING (3)
#define BLOCKSTATE_SWITCHING (4)
#define BLOCKSTATE_POPPING (5)
#define BLOCKSTATE_WAITING (6)

#define GAMEKEY_LEFT (0)
#define GAMEKEY_UP (1)
#define GAMEKEY_RIGHT (2)
#define GAMEKEY_DOWN (3)
#define GAMEKEY_SWITCH (4)
#define GAMEKEY_STACK (5)

#define KEYTYPE_DOWN (0)
#define KEYTYPE_UP (1)
#define KEYTYPE_REPEAT (2)

typedef unsigned char byte;

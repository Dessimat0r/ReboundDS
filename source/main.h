#ifndef __main_h__
#define __main_h__

#include "string.h"
#include <math.h>

#define VERSION "0.1"
#define NAME "Overlord ReboundDS"
#define NAME_AND_VER NAME " " VERSION
#define BUILD_TIME "Build time: " __TIME__
#define BUILD_DATE "Build date: " __DATE__

#ifndef NOMINMAX
  #ifndef max
    #define MAX(a,b) (((a) > (b)) ? (a) : (b))
  #endif
  #ifndef min
    #define MIN(a,b) (((a) < (b)) ? (a) : (b))
  #endif
  #ifndef limit
	/* limit a to a >= b && a <= c */
    #define LIMIT(a,b,c) (MIN(MAX(a, b),c))
  #endif
#endif  /* NOMINMAX */

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))

#include <PA9.h>

// Converted using PAGfx
#include "gfx/all_gfx.h"

#define DISPLAY_X_SIZE 256
#define DISPLAY_Y_SIZE 192

#define GAME_DISPLAY_X_SIZE  DISPLAY_X_SIZE
#define GAME_DISPLAY_Y_SIZE  168

#define GAME_DISPLAY_X_SIZE2 (GAME_DISPLAY_X_SIZE >> 1)
#define GAME_DISPLAY_Y_SIZE2 (GAME_DISPLAY_Y_SIZE >> 1)

#define GAME_DISPLAY_X_SIZE4 (GAME_DISPLAY_X_SIZE >> 2)
#define GAME_DISPLAY_Y_SIZE4 (GAME_DISPLAY_Y_SIZE >> 2)

#ifdef __cplusplus
extern "C" {
#endif
               
#ifdef ARM9
#include <nds.h>
#endif

#ifndef ARM9
#define RGB15(r,g,b) (((b)<<10)|((g)<<5)|(r))
#endif

#define RGB15A(r, g, b) ((RGB15(r, g, b)) | 0x8000)
#define RBG15A(r, b, g) ((RGB15(r, g, b)) | 0x8000)
#define GRB15A(g, r, b) ((RGB15(r, g, b)) | 0x8000)
#define GBR15A(g, b, r) ((RGB15(r, g, b)) | 0x8000)
#define BRG15A(b, r, g) ((RGB15(r, g, b)) | 0x8000)
#define BGR15A(b, g, r) ((RGB15(r, g, b)) | 0x8000)

s32 Modulo(s32 src, s32 divisor);
u32 IntDiv(u32 src, u32 divisor);   
s32 IntDivS(s32 src, s32 divisor);
u32 IntDivR(u32 src, u32 divisor);
void ParseIntIntoComponents(u32 input, u32 *tens, u32 *units);

int Max(int a, int b);
int Min(int a, int b);

#define Abs(x) ((x >= 0) ? (x) : (0-x))

u32 RangeWrap(s32 n, u32 greatest);

u16 rng_rand(u16 rng);

int FloatRound(float a);
#define RandomInt(x) rng_rand(x)

#define bits(src, bits) ((src & bits) == bits)
            
int ClosestPowerOfTwo(int n);
                  
#ifndef ARM9
#ifndef ARM7
int NearestXtoY(int x, int y);  
#endif
#endif

int ToTheNearestStride(int n, int stride, int offset);

int IntSqrt(int n);

// This function dereferences a pointer to a 32 bit value. Yeah, for some reason
// the DS doesn't like to dereference pointers that aren't on a word boundary.
static inline u32 Dereference32Bit(const u32 *value) {
   return (((const u8*)(value))[3] << 24)
        | (((const u8*)(value))[2] << 16)
        | (((const u8*)(value))[1] <<  8)
        | (((const u8*)(value))[0] <<  0);
}

static inline int max(int a, int b) {return a > b ? a : b;};
static inline int min(int a, int b) {return a < b ? a : b;};

#ifdef __cplusplus
}
#endif

unsigned char LINES_PARALLEL = 0, LINES_COINCIDENT = 1, LINES_NOT_INTERESECTING = 2, LINES_INTERESECTING = 3;

struct RDSPointF {
	float x, y;
};

struct RDSLineF {
	RDSPointF s, e;
};

struct RDSCircleF {
	RDSPointF pos;
	float r;
};

struct RDSRectI {
	int x1, y1, x2, y2;
};

bool clIntersect(const RDSCircleF &c, const RDSLineF &l);

#define COLOUR_GREY 0
#define COLOUR_RED 1
#define COLOUR_BLUE 2
#define COLOUR_DARK_BLUE 3
#define COLOUR_LIGHT_BLUE 4
#define COLOUR_YELLOW 5
#define COLOUR_GREEN 6
#define COLOUR_ORANGE 7

#define RECT_SIDE_NONE 0
#define RECT_SIDE_TOP 1
#define RECT_SIDE_BOTTOM 2
#define RECT_SIDE_LEFT 3
#define RECT_SIDE_RIGHT 4
#define AXIS_NONE 0
#define AXIS_X 1
#define AXIS_Y 2
#define INTERSECT_NONE 0
#define INTERSECT_WALLS 1
#define INTERSECT_BAT 2

const unsigned int
	BAT_WIDTH = 3U,
	BAT_HEIGHT = 20U,
	BAT_HALF_HEIGHT = BAT_HEIGHT / 2,
	BRICK_WIDTH = 8U,
	BRICK_HEIGHT = 12U,
	PLAY_FIELD_X = 10U,
	PLAY_FIELD_Y = 10U,
	BRICK_FIELD_BRICK_COLS = 25U,
	BRICK_FIELD_BRICK_ROWS = 13U,
	PLAY_FIELD_WIDTH = 230U,
	PLAY_FIELD_HEIGHT = 156U,
	PLAY_FIELD_X2 = PLAY_FIELD_X + (PLAY_FIELD_WIDTH - 1),
	PLAY_FIELD_Y2 = PLAY_FIELD_Y + (PLAY_FIELD_HEIGHT - 1),
	BRICK_FIELD_X_PLAY_FIELD_OFFSET = 30U,
	BRICK_FIELD_X = PLAY_FIELD_X + BRICK_FIELD_X_PLAY_FIELD_OFFSET,
	BRICK_FIELD_WIDTH = (PLAY_FIELD_WIDTH - 1) - BRICK_FIELD_X,
	BAT_X = PLAY_FIELD_X,
	BAT_AREA_Y = PLAY_FIELD_Y - 1,
	BAT_AREA_HEIGHT = BAT_AREA_Y + PLAY_FIELD_HEIGHT + 1,
	BALL_DIAMETER = 3,
	BALL_C_OFFSET = 2,
	MAX_NO_OF_BRICK_GROUPS = 8,
	//MAX_NO_OF_BRICKS_PER_BRICK_GROUP = 20,
	MAX_FORCEFIELD_LEVEL = 10,
	MSG_TILE_START_ROW = 22,
	MSG_TILE_START_COL = 10,
	P1_TEXT_START_ROW = 21,
	P1_TEXT_START_COL = 1,
	P1_SCORE_START_COL = 5,
	P2_TEXT_START_ROW = 21,
	P2_TEXT_START_COL = 13,
	P2_SCORE_START_COL = 17,
	LEVEL_INDICATOR_START_COL = 1,
	LEVEL_INDICATOR_START_ROW = 1,
	FPS_INDICATOR_START_COL = 1,
	FPS_INDICATOR_START_ROW = 3,
	FPS_TEXT_START_COL = 6,
	MAX_BRICKS_PER_LEVEL = 80,
	DEBUG_TEXT_START_COL = 1,
	DEBUG_TEXT_START_ROW = 10,
	FORCE_BIT_START_COL = 13,
	FORCE_BIT_START_ROW = 1
;

const unsigned char
	MAX_SCORE_LINE_CHAR = 5
;

bool rectIntersectsRectI(
	int aX1, int aY1, int aX2, int aY2, 
	int bX1, int bY1, int bX2, int bY2
);

bool lineIntersectsLineF(
	RDSLineF line1, RDSLineF line2,
	unsigned char &status,
	RDSPointF &intersection
);

bool rectIntersectsRectF(
	float aX1, float aY1, float aX2, float aY2, 
	float bX1, float bY1, float bX2, float bY2
);

bool getSpriteInUse(u8 screen, u8 obj_number);

// info for brick groups in the entire game
struct GeneralBrickGroupInfo {
	GeneralBrickGroupInfo() {}

	GeneralBrickGroupInfo(const u8 pal, const char *_title, const u16 *palData, const u8 *palData8) :
		pal(pal), palData(palData), palData8(palData8)
	{
		strcpy(title, _title);
	}

	u8 pal; // brick group palette
	char title[10]; // brick group title
	const u16 *palData;
	const u8 *palData8;
};

// info for brick groups in a level -- the array that contains these
// should have no inactive groups
struct LevelBrickGroupInfo {
	unsigned int generalGroupRef; // link to general group
	unsigned int bricks; // no of bricks in group
	unsigned int points;
	
	LevelBrickGroupInfo() {}
	
	LevelBrickGroupInfo(
		unsigned int generalGroupRef,
		unsigned int points
	) :
		generalGroupRef(generalGroupRef),
		bricks(0), // set this later
		points(points)
	{
	}
};

extern GeneralBrickGroupInfo generalBrickGroupInfos[MAX_NO_OF_BRICK_GROUPS];

#include "game.h"
#include "level.h"
#include "titlescr.h"

#endif

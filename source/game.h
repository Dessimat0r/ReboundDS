#ifndef __game_h__
#define __game_h__

#include "hud.h"
#include "level.h"

const u8 TOP_SCREEN = 1, BOTTOM_SCREEN = 0;

const char* FIELD_HIT_MESSAGES_10_7[] = {
	"Yeah!",
	"Awesome!",
	"Rocking!",
	"Ace!",
};

unsigned short FIELD_HIT_MESSAGES_10_7_LEN = ARR_LEN(FIELD_HIT_MESSAGES_10_7);

const char* FIELD_HIT_MESSAGES_6_3[] = {
	"Keep it up!",
	"Getting there!",
	"Totally badass",
	"Rocking out",
	"Totally sweet",
	"Bodacious"
};

unsigned short FIELD_HIT_MESSAGES_6_3_LEN = ARR_LEN(FIELD_HIT_MESSAGES_6_3);

const char* FIELD_HIT_MESSAGES_2_0[] = {
	"Almost there!",
	"Make it count",
	"Aww yeah!",
	"Leeeroy Jennkins!",
	"Just about..."
};

unsigned short FIELD_HIT_MESSAGES_2_0_LEN = ARR_LEN(FIELD_HIT_MESSAGES_2_0);

const u8
	BRICK_GROUP_GREY_PAL = 0,
	BRICK_GROUP_RED_PAL = 1,
	BRICK_GROUP_BLUE_PAL = 2,
	BRICK_GROUP_LIGHT_BLUE_PAL = 3,
	BRICK_GROUP_DARK_BLUE_PAL = 4
;

struct Level;
struct LevelSet;

struct BrickRef {
	unsigned char row, col;
};

#pragma pack(1)
struct BrickData {
	bool active;
	unsigned char type;
	u8 spriteID;
	unsigned char specialIndex;
};
#pragma pack()

struct BrickGroupRef {
	bool inUse;
	BrickRef ref;
};

struct Player {
	u8 id;
};

struct BrickCollision {
	unsigned char row, col;
	bool sidesHit[4];
	int noOfSidesHit;
};

struct BrickCollisions {
	unsigned char noOfCollisons;
	BrickCollision collisions[4];
};

struct Ball {
	float x, y;
	float vX, vY;
	bool active, visible;
};

struct Bat {
	// top y, bottom y is cached from top y
	int topY, bottomY;
};

// data for each brick group in a game
struct BrickGroupData {
	//BrickGroupData() {}

	//BrickGroupData(unsigned int activeBricks) :
	//activeBricks(activeBricks)
	//{}

	// no reference to LevelBrickGroupInfo is needed;
	// the array positions are the same.
	unsigned int activeBricks; // no of active bricks in group

	char pointsText[5];
	unsigned char pointsTextLength;
};

struct MultibrickData {
	unsigned char hits;
};

struct LevelData {
	bool inUse;

	//BrickData paBrickData[BRICK_FIELD_BRICK_ROWS * BRICK_FIELD_BRICK_COLS];

	BrickGroupData brickGroupData[MAX_NO_OF_BRICK_GROUPS];
	MultibrickData multiBrickData[30];
	BrickData brickData[MAX_BRICKS_PER_LEVEL];

	unsigned char noOfBricks;
};


struct BrickHitExtraData {
	bool bounceOffBrick;
	bool brickDisappeared;
};

class Game {
	public:
		Game();
		Game(LevelSet *_pLevelSet, LevelData *_pLevelDatas);
		~Game();

		void initGfx();
		void update();

		void updateBats();
		void updateBalls();
		void updateBall(unsigned int num, unsigned int iter = 0);
		void reboundBall(unsigned int num, unsigned int side);
		unsigned char& refBrickDataRef(unsigned char row, unsigned char col);

		bool testIntersectionWalls(
			int x1, int y1, int x2, int y2,
			unsigned char &side
		);

		bool testIntersectionBricks(
			float x1, float y1, float x2, float y2,
			unsigned char &brickRow, unsigned char &brickCol,
			unsigned char &brickNo,
			unsigned char &side
		);

		bool testIntersectionBricks2(
			unsigned char ballNo,
			float x1, float y1,
			float x2, float y2,
			float vX, float vY,
			BrickCollisions &bcs
		);

		void getFieldAreaIntersectRectLimited(
			float x1, float x2,
			float y1, float y2,
			RDSRectI &fieldRect
		);

		bool testIntersectionBat(
			int x1, int y1, int x2, int y2
		);
		
		void strengthenBarrier();
		void degradeBarrier();

		void resurrectBrickGroup(unsigned char group);

		bool spawnBrick(unsigned char row, unsigned char col);
		void removeBrick(unsigned char row, unsigned char col);
		void countBrick(unsigned char row, unsigned char col);
		
		void drawText(const u8 scr, u8 bg, const char* text, const s16 x, const s16 y, const unsigned char colour=COLOUR_GREY, const unsigned char minWidth=0);
		void drawChar(const u8 scr, u8 bg, const char c, const s16 x, const s16 y, const unsigned char colour=COLOUR_GREY);
		void drawCharRowCol(const u8 scr, u8 bg, const s16 x, const s16 y, const unsigned char row, const unsigned char col);
		
		void nextLevel();
		void initLevel();
		void setupLevelGfx();
		void disposeLevel();
		void resetBall();
		void preSetupLevel();
		void setupLevel();
		void updateScoreText(const bool removeOld=true);
		void updateBallsGfx(const bool removeOld=true);
		void updateLevelNoText(const bool removeOld=true);
		void bounceBall(const unsigned char ballNo, const unsigned char axis);
		void processBrickHit(const unsigned int row, const unsigned col, const unsigned char ball, const unsigned char side, BrickHitExtraData &data);
		void fallBack();
		void batBounce(const unsigned char batIndex, const unsigned char ballIndex, const float distanceFromTop);
		bool findBrickSideHit(unsigned int row, unsigned int col, float ballX, float ballY, unsigned char &side);
		void restoreLevel();
		void showMessage(const char* m_msg, const unsigned char colour=COLOUR_GREY);
		void displayFieldHitMsg();

		void initLevelGfx();
		bool createBrickGfx(unsigned char row, unsigned char col);
		void pulldownLevel();
		void initBrickGroupGfx(bool removeOld=true);
		void initForceGfx(bool removeOld=true);
		void updateForceGfx(bool removeOld=true);

		void initBrickGroupData(unsigned char index);
		void gameOver();
		void updateFPSText(bool removeOld=true);
		void showDebugMsg(const char *pMsg, const unsigned char line=0);

		bool testBallIntersection(unsigned char ballNo, unsigned char &row, unsigned char &col, unsigned char &side);

		bool placeBall(
			const unsigned char ballIndex,
			const unsigned int brickRow,
			const unsigned int brickCol,
			const unsigned char side
		);

		void printDebugGroupData();

		void blit8BitImgToBG(const u8 screen, const u8* data, const int srcX, const int srcY, const int srcX2, const int srcY2, const int srcImgWidth, const int srcImgHeight, const int dstX, const int dstY, const unsigned char palShift);
		void clearBGArea(const u8 screen, const int dstX, const int dstY, const int dstX2, const int dstY2);

		//bool getBrickSpriteInUse(u8 screen, u8 obj_number);

		u8 lastFPSUpdateTime;
		u8 fps;
		u8 brickGroupHitCounter;

		unsigned char brickGroupHit;

		bool inited;
		LevelSet *pLevelSet;
		LevelData *pLevelDatas;
		unsigned char currLevelNo;
		Level *pCurrLevel;
		unsigned char paBrickDataRef[BRICK_FIELD_BRICK_ROWS * BRICK_FIELD_BRICK_COLS];
		bool running;
		unsigned int score;
		LevelData ld[10];

		bool spriteUsed[127];
		
		HUD hud;
		Ball balls[1];
		Player players[1];
		Bat bats[1];

		u16 batGfx, ballGfx, brick1Gfx, brick2Gfx, gemGfx;
		u16 brick1Gfx_2, brick2Gfx_2;

		unsigned int forceFieldLevel;
		s8 flashCounter;

		unsigned long msgTicks;
		char msg[20];
		u8 msgLen;

		unsigned char ballsLeft;

		u16 crazyForceUpdateTick;
		u16 crazyRand;

		unsigned char brickGfxCount;


};

void SetupLevelData(LevelData &levelData);

#endif

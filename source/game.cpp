#ifdef __main__
#include "game.h"

extern u32 vbl_count;

Game::Game() :
	inited(false)
{}

Game::Game(LevelSet *_pLevelSet, LevelData *_pLevelDatas) :
	inited(true),
	pLevelSet(_pLevelSet),
	pLevelDatas(_pLevelDatas),
	currLevelNo(0),
	pCurrLevel(&pLevelSet->paLevel[currLevelNo]),
	running(true),
	lastFPSUpdateTime(0),
	fps(0),
	crazyForceUpdateTick(0),
	flashCounter(0),
	msgTicks(0),
	score(0),
	ballsLeft(3),
	msgLen(0),
	crazyRand(0),
	brickGfxCount(0),
	brickGroupHit(255),
	brickGroupHitCounter(0)
{
	//PA_ResetSpriteSys();
	//PA_Init16bitBg(0, 0);

	//memcpy(
	//	paBrickTypes,
	// pCurrLevel->paBrickTypes,
	//	sizeof(unsigned int) *
	//	(BRICK_FIELD_BRICK_ROWS * BRICK_FIELD_BRICK_COLS)
	//);

	//dmaCopy(
	//	pCurrLevel->paBrickTypes,
	//	paBrickTypes,
	//	sizeof(unsigned int) * (BRICK_FIELD_BRICK_ROWS * BRICK_FIELD_BRICK_COLS)
	//);
	
	Bat &bat = bats[0];
	Ball &ball = balls[0];

	bat.topY = 40;
	bat.bottomY = bat.topY + (BAT_HEIGHT - 1);
	hud.pGame = this;

	initGfx();
	initLevel();
	resetBall();

	//PA_InitText(0, 2);
	//PA_InitBg(1, )

	//PA_InitText(0, 0);

	//PA_SetTextCol(31, 31, 31, 0);

	//PA_BGScrollXY(0, 0, -5, -5);
	
	//PA_Print(0, "ReboundDS 0.1\n");
	//PA_OutputSimpleText(0, 0, 0, "ReboundDS 0.1");

	/*
	for (unsigned int i = 0; i < MAX_NO_OF_BRICK_GROUPS; i++) {
		PA_Print(
			0,
			"Brick group: %d, max: %d, num: %d\n",
			i, bricksPerGroup[i], activeBricksPerGroup[i]
		);
	}
	*/

	showDebugMsg(NAME_AND_VER, 0);
	showDebugMsg(BUILD_TIME, 1);
	showDebugMsg(BUILD_DATE, 2);
}

void Game::printDebugGroupData() {
	char str[30];

	for (unsigned char i = 0; i < pCurrLevel->noOfBrickGroups; i++) {
		BrickGroupData &bgd = pLevelDatas->brickGroupData[i];
		LevelBrickGroupInfo &lbgi = pCurrLevel->brickGroupInfos[i];
		unsigned char ref = lbgi.generalGroupRef;
		GeneralBrickGroupInfo &gbgi =  generalBrickGroupInfos[ref];

		sprintf(str, "%s: B:%d AB:%d P:%d", gbgi.title, lbgi.bricks, bgd.activeBricks < 50 ? bgd.activeBricks : 0, lbgi.points);
		showDebugMsg(str, i);
	}
}

void Game::initBrickGroupGfx(bool removeOld) {
	// iterate through brick scores and draw
	for (unsigned short i = 0; i < pCurrLevel->noOfBrickGroups; i++) {
		unsigned char bi_row = 3, bi_col = 0;

		unsigned int ref = pCurrLevel->brickGroupInfos[i].generalGroupRef;
		GeneralBrickGroupInfo &gbgi = generalBrickGroupInfos[ref];

		char pointsText[4];
		sprintf(pointsText, "%3d", pCurrLevel->brickGroupInfos[i].points);
		drawText(BOTTOM_SCREEN, 0, pointsText, 28, 1 + i, ref);

		/*
		
		PA_CreateSpriteFromGfx(
			0, // 0 = bottom screen
			sprindex, // sprite number (0 to 127)
			brick1Gfx_2, // sprite gfx
			OBJ_SIZE_8X16, // sprite size
			1, // 1 = 256 colours
			gbgi.pal, // palette number
			10, // x
			i * 10 // y
		);

		spriteUsed[sprindex++] = true;
		*/
	}
}

void Game::initLevelGfx() {
	// draw playfield lines, may as well do this again
	PA_Draw8bitLineEx(
		TOP_SCREEN,
		PLAY_FIELD_X, PLAY_FIELD_Y - 2,
		PLAY_FIELD_X2, PLAY_FIELD_Y - 2,
		14,
		2U
	);

	PA_Draw8bitLineEx(
		TOP_SCREEN,
		PLAY_FIELD_X, PLAY_FIELD_Y2 + 1,
		PLAY_FIELD_X2, PLAY_FIELD_Y2 + 1,
		14,
		2U
	);

	// side line
	PA_Draw8bitLineEx(
		TOP_SCREEN,
		PLAY_FIELD_X2 + 1, PLAY_FIELD_Y - 2,
		PLAY_FIELD_X2 + 1, PLAY_FIELD_Y2 + 1,
		14,
		2U
	);
	/*

	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			unsigned char bdref = paBrickDataRef[(row * BRICK_FIELD_BRICK_COLS) + col];
			if (bdref == 255) continue;
			BrickData &data = pLevelDatas[currLevelNo].brickData[bdref];

			if (data.active && data.spriteID == 255) {
				createBrickGfx(row, col);
			}
		}
	}
	*/

	initBrickGroupGfx(false);
}

void Game::updateFPSText(bool removeOld) {
	unsigned char i = 0;
	
	char msg[3];

	sprintf(msg, "%2d", fps);

	drawText(BOTTOM_SCREEN, 0, msg, FPS_TEXT_START_COL, FPS_INDICATOR_START_ROW, COLOUR_GREY, removeOld ? 2 : 0);
}

void Game::resetBall() {
	Ball &ball = balls[0];
	
	ball.x = 40.0f;
	ball.y = 40.0f;
	ball.vX = 1.0f;
	ball.vY = 1.0f;
	ball.visible = true;
	ball.active = true;

	PA_CreateSpriteFromGfx( 
		TOP_SCREEN, // 1 = top screen
		1, // sprite number (0 to 127)
		ballGfx, // sprite gfx
		OBJ_SIZE_8X8, // sprite size
		0, // 1 = 256 colours
		0, // palette number
		PLAY_FIELD_X + (int)ball.x, // x
		PLAY_FIELD_Y + (int)ball.y // y
	);

	PA_SetSpriteXY(
		1,
		1,
		PLAY_FIELD_X + (int)ball.x,
		PLAY_FIELD_Y + (int)ball.y
	);
}

void Game::initGfx() {
	// 1 = top screen
	// 0 = bottom screen

	PA_ResetSpriteSys();
	PA_ResetBgSys();

	// reset brick sprite usage
	//for (u8 i = 0; i < 128; i++) {
	//	spriteUsed[i] = false;
	//}

	//PA_Init8bitBg(0, 0);
	PA_Init8bitBg(TOP_SCREEN, 0); // initalise level border background

	// palette for level border bg
	//const u16 bgpal[4] __attribute__ ((aligned (4))) = {
	//PA_RGB8(0, 0, 0), PA_RGB8(15, 15, 15), PA_RGB8(31, 31, 31), PA_RGB8(31, 31, 31)};

	// load palette
	PA_Load8bitBgPal(TOP_SCREEN, (void*)bricksPal);

	PA_SetBgColor(TOP_SCREEN, PA_RGB(0, 0, 0));
	PA_SetBgColor(BOTTOM_SCREEN, PA_RGB(0, 0, 0));

	//PA_SetBgPalNCol(TOP_SCREEN, 0, 0, 0, PA_RGB(31, 31, 31));

	u8 *_bat_Sprite =
		const_cast<u8*>(bat_Sprite)
	;

	batGfx = PA_CreateGfx(
		TOP_SCREEN,
		_bat_Sprite,
		OBJ_SIZE_8X32,
		0
	);

	//u8 *_brick1_Sprite =
	//	const_cast<u8*>(brick1_Sprite)
	//;

	//brick1Gfx = PA_CreateGfx(
	//	TOP_SCREEN,
	//	_brick1_Sprite,
	//	OBJ_SIZE_8X16,
	//	1
	//);

	/*
	brick1Gfx_2 = PA_CreateGfx(
		BOTTOM_SCREEN,
		_brick1_Sprite,
		OBJ_SIZE_8X16,
		1
	);
	*/

	//u8 *_brick2_Sprite =
	//	const_cast<u8*>(brick2_Sprite)
	//;

	//brick2Gfx = PA_CreateGfx(
	//	TOP_SCREEN,
	//	_brick2_Sprite,
	//	OBJ_SIZE_8X16,
	//	1
	//);

	/*
	brick2Gfx_2 = PA_CreateGfx(
		BOTTOM_SCREEN,
		_brick2_Sprite,
		OBJ_SIZE_8X16,
		1
	);
	*/

	u8 *_ball_Sprite =
		const_cast<u8*>(ball_Sprite)
	;

	ballGfx = PA_CreateGfx(
		TOP_SCREEN,
		_ball_Sprite,
		OBJ_SIZE_8X8,
		0
	);

	//u8 *_gem1_Sprite =
	//	const_cast<u8*>(gem1_Sprite)
	//;

	//gemGfx = PA_CreateGfx(
	//	TOP_SCREEN,
	//	_gem1_Sprite,
	//	OBJ_SIZE_8X16,
	//	0
	//);

	
	/*
	// draw playfield lines
	PA_Draw16bitLine(
		1,
		PLAY_FIELD_X, PLAY_FIELD_Y,
		PLAY_FIELD_X2, PLAY_FIELD_Y,
		PA_RGB(31, 0, 31)
	);

	PA_Draw16bitLine(
		1,
		PLAY_FIELD_X, PLAY_FIELD_Y2,
		PLAY_FIELD_X2, PLAY_FIELD_Y2,
		PA_RGB(31, 0, 31)
	);

	PA_Draw16bitLine(
		1,
		PLAY_FIELD_X2, PLAY_FIELD_Y,
		PLAY_FIELD_X2, PLAY_FIELD_Y2,
		PA_RGB(31, 0, 31)
	);
	*/

	//PA_LoadSpritePal(TOP_SCREEN, 0, (void*)bat_Pal); // Load the bat sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 1, (void*)ball_Pal); // Load the ball sprite palette

	PA_LoadSprite16cPal(TOP_SCREEN, 0, (void*)pal1_Pal); // Load the grey sprite palette (16-colour)

	//PA_LoadSpritePal(0, 0, (void*)pal1_Pal); // Load the sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 1, (void*)pal1_Pal); // Load the grey sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 2, (void*)pal2_Pal); // Load the red sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 3, (void*)pal3_Pal); // Load the blue sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 4, (void*)pal4_Pal); // Load the dark blue sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 5, (void*)pal5_Pal); // Load the light blue sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 6, (void*)pal6_Pal); // Load the yellow sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 7, (void*)pal7_Pal); // Load the green sprite palette
	//PA_LoadSpritePal(TOP_SCREEN, 8, (void*)pal8_Pal); // Load the orange sprite palette
	
	PA_CreateSpriteFromGfx( 
		TOP_SCREEN, // 1 = top screen
		0, // sprite number (0 to 127)
		batGfx, // sprite gfx
		OBJ_SIZE_8X32, // sprite size
		0, // 1 = 256 colours
		0, // palette number
		BAT_X, // x
		BAT_AREA_Y + bats[0].topY // y
	);

	Ball &ball = balls[0];

	PA_CreateSpriteFromGfx( 
		TOP_SCREEN, // 1 = top screen
		1, // sprite number (0 to 127)
		ballGfx, // sprite gfx
		OBJ_SIZE_8X8, // sprite size
		0, // 1 = 256 colours
		0, // palette number
		PLAY_FIELD_X + (int)ball.x, // x
		PLAY_FIELD_Y + (int)ball.y // y
	);

	PA_LoadBgTiles(BOTTOM_SCREEN, 0, font_Tiles);
	PA_LoadBgMap(BOTTOM_SCREEN, 0, const_cast<u16*>(font_Map), BG_256X256);
	PA_LoadBgPal(BOTTOM_SCREEN, 0, const_cast<u16*>(font_Pal));
	PA_InitBg(BOTTOM_SCREEN, 0, BG_256X256, 0, 1);
	PA_ClearBg(BOTTOM_SCREEN, 0);

	PA_LoadBgTiles(TOP_SCREEN, 0, font_Tiles);
	PA_InitBg(TOP_SCREEN, 0, BG_256X256, 0, 1);
	PA_LoadBgMap(TOP_SCREEN, 1, const_cast<u16*>(font_Map), BG_256X256);
	PA_LoadBgPal(TOP_SCREEN, 0, const_cast<u16*>(font_Pal));
	PA_ClearBg(TOP_SCREEN, 0);

	/*
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 24; j++) {
			PA_SetMapTile(TOP_SCREEN, 0, i, j, 255);
			PA_SetMapTile(BOTTOM_SCREEN, 0, i, j, 255);
		}
	}
	*/

	PA_ClearBg(BOTTOM_SCREEN, 0);

	PA_LoadBgTiles(BOTTOM_SCREEN, 1, bg1_Tiles);
	PA_LoadBgMap(BOTTOM_SCREEN, 1, const_cast<u16*>(bg1_Map), BG_256X256);
	PA_InitBg(BOTTOM_SCREEN, 1, BG_256X256, 0, 1);
	PA_LoadBgPal(BOTTOM_SCREEN, 1, const_cast<u16*>(bg1_Pal));

	drawText(BOTTOM_SCREEN, 0, "1UP", P1_TEXT_START_COL, P1_TEXT_START_ROW, COLOUR_YELLOW);
	drawText(BOTTOM_SCREEN, 0, "2UP", P2_TEXT_START_COL, P2_TEXT_START_ROW, COLOUR_YELLOW);
	drawText(BOTTOM_SCREEN, 0, "0", P2_SCORE_START_COL, P2_TEXT_START_ROW, COLOUR_GREY);
	drawText(BOTTOM_SCREEN, 0, "FPS:", FPS_INDICATOR_START_COL, FPS_INDICATOR_START_ROW, COLOUR_GREY);
	initForceGfx();
	
	updateScoreText(false);
	updateBallsGfx(false);
	updateFPSText(false);

	//drawCharRowCol(26, 1 + i, bi_row, bi_col + 1);
}

Game::~Game() {
	inited = false;
}

void Game::blit8BitImgToBG(const u8 screen, const u8* data, const int srcX, const int srcY, const int srcX2, const int srcY2, const int srcImgWidth, const int srcImgHeight, const int dstX, const int dstY, const unsigned char palShift) {
	int src_x, src_y, dst_x = dstX, dst_y = dstY, srcW = (srcX2 - srcX) + 1, srcH = (srcY2 - srcY) + 1;

	for (src_y = srcY, dst_y = dstY; src_y <= srcY2; src_y++, dst_y++) {
		for (src_x = srcX, dst_x = dstX; src_x <= srcX2; src_x++, dst_x++) {
			u16 d = data[(src_y * srcImgWidth) + src_x];
			if (d != 0) {
				PA_Put8bitPixel(screen, dst_x, dst_y, (palShift * 16) + d);
			}
		}
	}
}

void Game::clearBGArea(const u8 screen, const int dstX, const int dstY, const int dstX2, const int dstY2) {
	int dst_x = dstX, dst_y = dstY;

	for (dst_y = dstY; dst_y < dstY2; dst_y++) {
		for (dst_x = dstX; dst_x < dstX2; dst_x++) {
			PA_Put8bitPixel(screen, dst_x, dst_y, 0);
		}
	}
}

void Game::updateLevelNoText(const bool removeOld) {
	char text[4];
		
	sprintf(text, "1-%1d", currLevelNo+1);
	unsigned char len = strlen(text);

	drawText(BOTTOM_SCREEN, 0, text, LEVEL_INDICATOR_START_COL, LEVEL_INDICATOR_START_ROW, COLOUR_YELLOW, 3);
}

void Game::batBounce(const unsigned char batIndex, const unsigned char ballIndex, const float distanceFromTop) {
	unsigned char segment = 1;
	
	if (distanceFromTop < 4.0f) segment = 0;
	else if (distanceFromTop < 14.0f) segment = 1;
	else if (distanceFromTop < 20.0f) segment = 2; // y goes from 0 to 19

	float adder = 0.0f;
	float multiplier = -1.0f;

	if (segment == 0 || segment == 2) {
		if ((segment == 0 && balls[ballIndex].vY < 0.0f) || (segment == 2 && balls[ballIndex].vY > 0.0f)) {
			multiplier = 1.0f;
		}
		float endDistance = 4.0f - ((segment == 0) ? distanceFromTop : BAT_HEIGHT - 1 - distanceFromTop);
		adder += ((0.5f / 5.0f) * endDistance);
	}

	float rand = -0.3f + (PA_RandMax(60) / 100.0f);
	multiplier += rand;

	balls[ballIndex].vX *= -1.0f;
	balls[ballIndex].vY = (balls[ballIndex].vY + adder) * multiplier;
}

/*
void Game::batBounce(const unsigned char batIndex, const unsigned char ballIndex, const float distanceFromTop) {
	unsigned char segment = 1;
	
	if (distanceFromTop < 5) segment = 0;
	else if (distanceFromTop < 14) segment = 1;
	else if (distanceFromTop < 20) segment = 2; // y goes from 0 to 19
	
	float radians;
	float angle; // goes from 0 - 511, 512 values
	float multiplier;

	if (segment == 1) {
		angle = 64.0f;
		radians = 0.25f;
	} else if (segment == 0 || segment == 2) {
		float endDistance = 5.0f - ((segment == 0) ? distanceFromTop : BAT_HEIGHT - 1 - distanceFromTop);
		float angleRange = 32.0f; // 20~ degrees
		//float radRange = 0.25f;
		//radians = 0.25f + ((radRange / 5) * endDistance);
		angle = 64.0f + ((angleRange / 5) * endDistance) * (segment == 0 ? 1.0f : -1.0f);
	}
	
	//float randRandians = (-100 + PA_RandMax(200)) / 1000.0f; // between -0.1 and 0.1.
	int randDegress = -16 + PA_RandMax(32);
	//radians += randRandians;
	angle += randDegress;

	balls[ballIndex].vY = PA_Cos((int)angle) * 0.005f; // scale to vX
	balls[ballIndex].vX = 1.0f;
}
*/

void Game::initBrickGroupData(unsigned char groupIndex) {
	BrickGroupData &bgd = pLevelDatas[currLevelNo].brickGroupData[groupIndex];
	LevelBrickGroupInfo &gbgd = pCurrLevel->brickGroupInfos[groupIndex];
	sprintf(bgd.pointsText, "%d", gbgd.points);
	bgd.pointsTextLength = strlen(bgd.pointsText);
	bgd.activeBricks = 0;
}

void Game::updateBats() {
	if (Stylus.Held || Stylus.Newpress) {
		Bat &bat = bats[0];

		// all this needs fixing
		
		int yborder = 10;
		int totalyborder = yborder * 2;
		int yrange = SCREEN_HEIGHT - totalyborder;

		int stylusYNew = MAX(
			yborder,
			MIN(
				Stylus.Y,
				SCREEN_HEIGHT - yborder
			)
		);

		int batcy = (int)(((
			(float)(stylusYNew / (float)yrange)
		)) * (float)BAT_AREA_HEIGHT);

		batcy = MAX(
			(BAT_HEIGHT / 2), // fix this
			MIN(
				batcy,
				(BAT_AREA_HEIGHT - 1) - (BAT_HEIGHT / 2) - 1 - 8 // and this!
			)
		);

		int baty = batcy - ((BAT_HEIGHT / 2) - 1);
		
		bat.topY = baty;
		bat.bottomY = bat.topY + (BAT_HEIGHT - 1);
		
		// update bat sprite
		PA_SetSpriteY(1, 0, BAT_AREA_Y + bat.topY);
	}
}

void Game::updateBalls() {
	updateBall(0);
}

void Game::degradeBarrier() {
	
}

void Game::strengthenBarrier() {

}

void Game::displayFieldHitMsg() {
	char** paMessages;
	unsigned char noOfMsgs;

	if (forceFieldLevel > 6) {
		paMessages = const_cast<char **>(FIELD_HIT_MESSAGES_10_7);
		noOfMsgs = FIELD_HIT_MESSAGES_10_7_LEN;
	} else if (forceFieldLevel > 2) {
		paMessages = const_cast<char **>(FIELD_HIT_MESSAGES_6_3);
		noOfMsgs = FIELD_HIT_MESSAGES_6_3_LEN;
	} else {
		paMessages = const_cast<char **>(FIELD_HIT_MESSAGES_2_0);
		noOfMsgs = FIELD_HIT_MESSAGES_2_0_LEN;
	}

	unsigned char rand = PA_RandMax(noOfMsgs - 1);
	char* msg = paMessages[rand];
	showMessage(msg);
}

void Game::resurrectBrickGroup(unsigned char group) {
	// TODO: OPTIMISE!

	//PA_Print(0, "Resurrecting brick group\n");

	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			unsigned char bdref = refBrickDataRef(row, col);
			if (bdref == 255) continue;

			BrickData &brickData = pLevelDatas[currLevelNo].brickData[bdref];

			const unsigned char &type = brickData.type;
			const unsigned char &ltype =
				pCurrLevel->paBrickTypes[(row * BRICK_FIELD_BRICK_COLS) + col]
			;
			if (!brickData.active) {
				unsigned char realType;
				bool genericBrick = false;
				unsigned char _group;

				if (ltype >= 20) {
					genericBrick = true;
					GetGenericBrickGroupAndType(
						ltype,
						_group,
						realType
					);

					if (_group == group) {
						spawnBrick(row, col);
					}
				} else {
					realType = ltype;
				}
			}
		}
	}
}

void Game::removeBrick(unsigned char row, unsigned char col) {
	// get brick data
	unsigned char bdref = refBrickDataRef(row, col);
	if (bdref == 255) return;

	BrickData &brickData = pLevelDatas[currLevelNo].brickData[bdref];
	unsigned const int type = brickData.type;
	
	//spriteUsed[brickData.spriteID] = false;
	//PA_DeleteSprite(1, brickData.spriteID);

	clearBGArea(
		TOP_SCREEN,
		BRICK_FIELD_X + (BRICK_WIDTH * col),
		PLAY_FIELD_Y + (BRICK_HEIGHT * row),
		BRICK_FIELD_X + (BRICK_WIDTH * (col + 1)),
		PLAY_FIELD_Y + (BRICK_HEIGHT * (row + 1))
	);
	
	countBrick(row, col);
}

bool Game::createBrickGfx(unsigned char row, unsigned char col) {
	brickGfxCount++;

	const unsigned char &ltype =
		pCurrLevel->paBrickTypes[(row * BRICK_FIELD_BRICK_COLS) + col]
	;
	unsigned char bdref = refBrickDataRef(row, col);
	if (bdref == 255) {
		return false;
	}

	BrickData &bData = pLevelDatas[currLevelNo].brickData[bdref];
	unsigned const char &type = bData.type;

	unsigned char rtype;
	unsigned char realType = 255;
	unsigned char group = 255;

	// get brick data
	if (ltype >= 20) {
		GetGenericBrickGroupAndType(
			ltype,
			group,
			realType
		);
		rtype = realType;
	} else {
		rtype = ltype;
	}
	
	u8 pal = 0;
	u16 spr = brick1Gfx;
	const u8 *data;
	const u16 *palData;
	const u8 *palData8;
	unsigned char palShift = 0;
	palData = pal1_Pal;
	int x, y, x2, y2;

	if (rtype == BRICK_TYPE_NORMAL) {
		spr = brick1Gfx;
		data = brick1_Sprite;
		x = 0; y = 0; x2 = 7; y2 = 11;
	} else if (rtype == BRICK_TYPE_HIT_TWICE_MANY) {
		spr = brick2Gfx;
		data = brick2_Sprite;
		x = 8; y = 0; x2 = 15; y2 = 11;
	} else if (rtype == BRICK_TYPE_GEM) {
		spr = gemGfx;
		data = gem1_Sprite;
		x = 16; y = 0; x2 = 23; y2 = 11;
	} else {
		spr = brick1Gfx;
		data = brick1_Sprite;
		x = 0; y = 0; x2 = 7; y2 = 11;
	}

	if (group != 255) {
		pal = generalBrickGroupInfos[
			pCurrLevel->brickGroupInfos[group].generalGroupRef
		].pal;
		palData = generalBrickGroupInfos[
			pCurrLevel->brickGroupInfos[group].generalGroupRef
		].palData;
		palData8 = generalBrickGroupInfos[
			pCurrLevel->brickGroupInfos[group].generalGroupRef
		].palData8;

		palShift = pCurrLevel->brickGroupInfos[group].generalGroupRef;
	}

	/*
	PA_CreateSpriteFromGfx(
		TOP_SCREEN, // 1 = top screen
		i, // sprite number (0 to 127)
		spr, // sprite gfx
		OBJ_SIZE_8X16, // sprite size
		1, // 1 = 256 colours
		pal, // palette number
		BRICK_FIELD_X + (BRICK_WIDTH * col), // x
		PLAY_FIELD_Y + (BRICK_HEIGHT * row) // y
	);
	*/
	
	/*
	blit8BitImgToBG(
		TOP_SCREEN,
		data,
		palData8,
		0, 0, 7, 11,
		BRICK_FIELD_X + (BRICK_WIDTH * col),
		PLAY_FIELD_Y + (BRICK_HEIGHT * row),
		PA_RGB8(16, 0, 16)
	);
	*/

	blit8BitImgToBG(
		TOP_SCREEN,
		bricksBitmap,
		x, y, x2, y2,
		64, 64,
		BRICK_FIELD_X + (BRICK_WIDTH * col),
		PLAY_FIELD_Y + (BRICK_HEIGHT * row),
		palShift
	);

	return true;
}

void Game::pulldownLevel() {
	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			unsigned char bdref = refBrickDataRef(row, col);
			if (bdref == 255) continue;
			BrickData &data = pLevelDatas[currLevelNo].brickData[bdref];

			if (data.active && data.spriteID != 0) {
				PA_Clear8bitBg(TOP_SCREEN);
				//PA_DeleteSprite(TOP_SCREEN, data.spriteID);
				//data.spriteID = 0;
			}
		}
	}
}

bool Game::spawnBrick(unsigned char row, unsigned char col) {
	unsigned char &ltype =
		pCurrLevel->paBrickTypes[(row * BRICK_FIELD_BRICK_COLS) + col]
	;
	unsigned char bdref = refBrickDataRef(row, col);
	if (bdref == 255) return false;

	BrickData &bData = pLevelDatas[currLevelNo].brickData[bdref];
	const unsigned char &type = bData.type;

	if (ltype == 0 || bData.active) return false;

	unsigned char rtype;
	unsigned char realType = 255;
	unsigned char group = 255;

	unsigned char side;

	unsigned char nrow, ncol;

	// test for ball collision with brick to be spawned
	bool ballinbrick = testBallIntersection((unsigned char)0, nrow, ncol, side);

	// get brick data
	if (ltype >= 20) {
		GetGenericBrickGroupAndType(
			ltype,
			group,
			realType
		);
		pLevelDatas[currLevelNo].brickGroupData[group].activeBricks++;
		
		if (!ballinbrick) {
			if (realType == BRICK_TYPE_HIT_TWICE_MANY) {
				pLevelDatas[currLevelNo].multiBrickData[bData.specialIndex].hits = 0;
			}
		}
	} else {
		realType = ltype;
	}

	bData.active = true;

	if (!ballinbrick) {
		createBrickGfx(row, col);
	} else {
		countBrick(row, col);
	}

	return true;
}

void Game::bounceBall(const unsigned char ballNo, const unsigned char axis) {
	if (!balls[ballNo].active) return;

	bool pos;

	if (axis == AXIS_X) balls[ballNo].vX *= -1.0f;
	else if (axis == AXIS_Y) balls[ballNo].vY *= -1.0f;

	if (balls[ballNo].vY >= 0) {
		pos = true;
	} else {
		pos = false;
	}
	
	PA_SRand(crazyRand += (crazyRand + 1) * 3);
	float randY = (PA_RandMax(1000) / 10000) - 0.05f; // range: 0.1f
	balls[ballNo].vY += randY;

	LIMIT(balls[ballNo].vX, -0.5f, 0.5f);

	if (pos) {
		LIMIT(balls[ballNo].vY, 0.1f, 0.5f);	
	} else {
		LIMIT(balls[ballNo].vY, -0.5f, -0.1f);
	}
}

void Game::updateScoreText(const bool removeOld) {
	if (score >= 100000) return;

	char sc[6];
		
	sprintf(sc, "%d", score);
	char len = strlen(sc);

	for (unsigned char i = 0; i < MAX_SCORE_LINE_CHAR; i++) {
		if (i < len) {
			drawChar(BOTTOM_SCREEN, 0, sc[i], P1_SCORE_START_COL + i, P1_TEXT_START_ROW);
		} else {
			if (removeOld) drawChar(BOTTOM_SCREEN, 0, ' ', P1_SCORE_START_COL + i, P1_TEXT_START_ROW);
		}
	}
}

bool Game::testBallIntersection(unsigned char ballNo, unsigned char &row, unsigned char &col, unsigned char &side) {
	Ball &ball = balls[ballNo];

	float newX = ball.x + ball.vX;
	float newY = ball.y + ball.vY;
	float newX2 = ball.x + BALL_DIAMETER - 1;
	float newY2 = ball.y + BALL_DIAMETER - 1;

	RDSLineF line1, line2, ballTravelLine;
	RDSRectI aoi;

	ballTravelLine.s.x = ball.x;
	ballTravelLine.s.y = ball.y;

	ballTravelLine.e.x = newX;
	ballTravelLine.e.y = newY;

	getFieldAreaIntersectRectLimited(
		ballTravelLine.s.x, ballTravelLine.e.x,
		ballTravelLine.s.y, ballTravelLine.e.y,
		aoi
	);

	float brickX = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (aoi.x1 * BRICK_WIDTH);
	float brickY = aoi.y1 * BRICK_HEIGHT;

	line1.s.x = brickX + (ball.vX > 0 ? 0 : BRICK_WIDTH - 1);
	line1.s.y = brickY;

	line1.e.x = brickX + (ball.vX > 0 ? 0 : BRICK_WIDTH - 1);
	line1.e.y = brickY + BRICK_HEIGHT - 1;

	line2.s.x = brickX;
	line2.s.y = brickY + (ball.vY > 0 ? 0 : BRICK_HEIGHT - 1);

	line2.e.x = brickX + BRICK_WIDTH - 1;
	line2.e.y = brickY + (ball.vY > 0 ? 0 : BRICK_HEIGHT - 1);

	unsigned char side1 = ball.vX > 0 ? RECT_SIDE_LEFT : RECT_SIDE_RIGHT;
	unsigned char side2 = ball.vY > 0 ? RECT_SIDE_TOP : RECT_SIDE_BOTTOM;

	unsigned char status;
	RDSPointF intersection;

	if (lineIntersectsLineF(ballTravelLine, line1, status, intersection)) {
		row = brickY;
		col = brickX;
		side = side1;
		return true;
	}
	
	if (lineIntersectsLineF(ballTravelLine, line2, status, intersection)) {
		row = brickY;
		col = brickX;
		side = side2;
		return true;
	}

	return false;
}

void Game::updateBall(unsigned int num, unsigned int iter) {
	Ball &ball = balls[num];

	if (!ball.active) return;

	float newX = ball.x + ball.vX;
	float newY = ball.y + ball.vY;
	float newX2 = ball.x + BALL_DIAMETER - 1;
	float newY2 = ball.y + BALL_DIAMETER - 1;

	unsigned char side = RECT_SIDE_NONE;


	// test for intersections
	bool intersectingWWalls = testIntersectionWalls(
		(int)newX, (int)newY,
		(int)newX2, (int)newY2,
		side
	);

	if (!intersectingWWalls) {
		bool intersectingWBat =
			testIntersectionBat(
				(int)newX, (int)newY,
				(int)newX2, (int)newY2
			)
		;
		
		if (!intersectingWBat) {
			unsigned char brickRow;
			unsigned char brickCol;
			unsigned char brickNo;

			unsigned char axis = AXIS_NONE;

			BrickCollisions bcs;

			/*
			bool intersectingWBricks =
				testIntersectionBricks2(
					num,
					newX, newY,
					newX2, newY2,
					ball.vX, ball.vY,
					bcs
				)
			;
			*/

			bool intersectingWBricks =
				(newX >= BRICK_FIELD_X_PLAY_FIELD_OFFSET) &&
				testIntersectionBricks(
					newX, newY,
					newX2, newY2,
					brickRow, brickCol,
					brickNo,
					side
				)
			;

			if (!intersectingWBricks) {
				ball.x = newX;
				ball.y = newY;

				PA_SetSpriteXY(
					1,
					1,
					PLAY_FIELD_X + (int)newX,
					PLAY_FIELD_Y + (int)newY
				);
			} else {
				switch (side) {
					case RECT_SIDE_LEFT: case RECT_SIDE_RIGHT:
						axis = AXIS_X; break;
					case RECT_SIDE_TOP: case RECT_SIDE_BOTTOM:
						axis = AXIS_Y; break;
				}

				//BrickCollision &bc = bcs.collisions[0];
				//side = RECT_SIDE_NONE;
				
				/*
				if (bc.sidesHit[0]) {
					side = RECT_SIDE_TOP;
				} else if (bc.sidesHit[1]) {
					side = RECT_SIDE_RIGHT;
				} else if (bc.sidesHit[2]) {
					side = RECT_SIDE_BOTTOM;
				} else if (bc.sidesHit[3]) {
					side = RECT_SIDE_LEFT;
				}
				*/

				unsigned char bdref = refBrickDataRef(brickRow, brickCol);
				if (bdref != 255) {
					BrickData &brickData = pLevelDatas[currLevelNo].brickData[bdref];
					
					unsigned char realType;
					bool genericBrick = false;
					unsigned char group;

					if (brickData.type >= 20) {
						genericBrick = true;
						GetGenericBrickGroupAndType(
							brickData.type,
							group,
							realType
						);
					}

					BrickHitExtraData data;

					processBrickHit(brickRow, brickCol, num, side, data);

					if (data.bounceOffBrick) {
						placeBall(num, brickRow, brickCol, side);
						bounceBall(num, axis);
					}
				}

				newX = ball.x + ball.vX;
				newY = ball.y + ball.vY;
				newX2 = ball.x + BALL_DIAMETER - 1;
				newY2 = ball.y + BALL_DIAMETER - 1;

				ball.x = newX;
				ball.y = newY;
				
				PA_SetSpriteXY(
					1,
					1,
					PLAY_FIELD_X + (int)newX,
					PLAY_FIELD_Y + (int)newY
				);

				//if (++iter < 2) updateBall(0, iter);
			}
		} else {
			/*
			ball.vX = -ball.vX;

			float rand = -7.0f + ((float)PA_RandMax(3) / 1000.0f);

			int v = ball.y + rand - bats[0].topY; // what is v?
			if (v >= -8.0f && (v <= 8.0f)) ball.vY = v/10.0f; // what is v?
			else ball.vY = v/4.0f;
			ball.vY = LIMIT(ball.vY, -10.0f, 10.0f);
			*/

			batBounce(0, num, ball.y - bats[0].topY);
			

			if (++iter < 2) updateBall(0, iter);
		}
	} else {
		bool perish = false;
		switch (side) {
			case RECT_SIDE_LEFT:
				perish = true;
				break;
			case RECT_SIDE_RIGHT:
				{
					bool bounce = true;
					if (forceFieldLevel > 0) {
						forceFieldLevel--;

						if (forceFieldLevel == 0) {
							// clear side line (paint black)
							// in the future, may wish to blit background image or something
							PA_Draw8bitLineEx(
								TOP_SCREEN,
								PLAY_FIELD_X2 + 1, PLAY_FIELD_Y - 2,
								PLAY_FIELD_X2 + 1, PLAY_FIELD_Y2 + 1,
								0,
								2U
							);
						} else {
							flashCounter = 31;
							displayFieldHitMsg();
							updateForceGfx(true);
						}
					} else {
						bounce = false;
						nextLevel();
					}
					if (bounce) bounceBall(0, AXIS_X);
				}
				break;
			case RECT_SIDE_TOP:
			case RECT_SIDE_BOTTOM:
				bounceBall(0, AXIS_Y);
				break;
		}

		if (perish) {
			ball.visible = false;
			ball.active = false;

			PA_DeleteSprite(1, 1);
		} else {
			//ball.x = (int)ball.x;
			//ball.y = (int)ball.y;

			newX = ball.x + ball.vX;
			newY = ball.y + ball.vY;
			newX2 = ball.x + BALL_DIAMETER;
			newY2 = ball.y + BALL_DIAMETER;

			ball.x = newX;
			ball.y = newY;

			PA_SetSpriteXY(
				1,
				1,
				PLAY_FIELD_X + (int)newX,
				PLAY_FIELD_Y + (int)newY
			);
			
			//if (++iter < 2) updateBall(0, iter);
		}
	}
}

void Game::processBrickHit(
	const unsigned int row, const unsigned col,
	const unsigned char ball, const unsigned char side,
	BrickHitExtraData &data
) {
	unsigned char axis = AXIS_NONE;
	
	switch (side) {
		case RECT_SIDE_LEFT: case RECT_SIDE_RIGHT:
			axis = AXIS_X; break;
		case RECT_SIDE_TOP: case RECT_SIDE_BOTTOM:
			axis = AXIS_Y; break;
	}

	// process brick hit
	unsigned char bdref = refBrickDataRef(row, col);
	if (bdref == 255) return;
	BrickData &brickData = pLevelDatas[currLevelNo].brickData[bdref];
	
	unsigned char realType = 255;
	bool genericBrick = false;
	unsigned char group = 255;

	if (brickData.type >= 20) {
		genericBrick = true;
		GetGenericBrickGroupAndType(
			brickData.type,
			group,
			realType
		);
	}

	unsigned char &type = brickData.type;
	bool remBrick = false;
	bool bounce = false;

	if (type != BRICK_TYPE_NONE) {
		bounce = true;
		remBrick = true;
		if (realType == BRICK_TYPE_HIT_TWICE_MANY) {
			MultibrickData &mbd = pLevelDatas[currLevelNo].multiBrickData[brickData.specialIndex];
			if (group == COLOUR_GREY) {
				// brick becomes a 'hit many' brick if grey, account for this!
				if (++mbd.hits < 10) remBrick = false;
			} else {
				if (++mbd.hits < 2) remBrick = false;
			}
		} else if (realType == BRICK_TYPE_GEM) {
			remBrick = true;
			bounce = false;
		}
	}

	if (remBrick) {
		data.brickDisappeared = true;
		removeBrick(row, col);
	}
	if (bounce) data.bounceOffBrick = true;
}

void Game::fallBack() {
	pulldownLevel();
	if ((currLevelNo - 1) > 0) {
		currLevelNo--;

		pCurrLevel = &pLevelSet->paLevel[currLevelNo];
		//paBrickData = pLevelDatas[currLevelNo].paBrickData;

		if (!pLevelDatas[currLevelNo].inUse) {
			initLevel();
		} else {
			restoreLevel();
		}
		balls[0].x = 180.0f;
	} else {
		// oops!
	}
}

void Game::nextLevel() {
	pulldownLevel();
	if ((currLevelNo + 1) < pLevelSet->noOfLevels) {
		currLevelNo++;

		pCurrLevel = &pLevelSet->paLevel[currLevelNo];
		//paBrickData = pLevelDatas[currLevelNo].paBrickData;

		if (!pLevelDatas[currLevelNo].inUse) {
			initLevel();
		} else {
			restoreLevel();
		}

		balls[0].x = 40.0f;
	} else {
		gameOver();
	}
}

void Game::gameOver() {
	showMessage("Game over! :(");
	running = false;

	balls[0].visible = false;
	balls[0].active = false;

	PA_DeleteSprite(1, 1);
}

void Game::initLevel() {
	LevelData &ld = pLevelDatas[currLevelNo];

	ld.inUse = true;

	for (unsigned char i = 0; i < pCurrLevel->noOfBrickGroups; i++) {
		initBrickGroupData(i);
	}
	
	// format brick data array
	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			refBrickDataRef(row, col) = 255; // reset
		}
	}
	
	unsigned char multibrickno = 0;
	ld.noOfBricks = 0; // TEMP BUG FIX

	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			unsigned char &type =
				pCurrLevel->paBrickTypes[(row * BRICK_FIELD_BRICK_COLS) + col]
			;

			if (type > 0) {
				refBrickDataRef(row, col) = ld.noOfBricks + 1;

				BrickData &brickData = ld.brickData[ld.noOfBricks + 1];
				brickData.active = false;
				brickData.type = type;
				brickData.spriteID = 255;
				brickData.specialIndex = 0;

				unsigned char realGroup = 255;
				unsigned char realType = 255;

				GetGenericBrickGroupAndType(type, realGroup, realType);

				if (realType == BRICK_TYPE_HIT_TWICE_MANY) {
					pLevelDatas[currLevelNo].multiBrickData[brickData.specialIndex].hits = 0;
					brickData.specialIndex = multibrickno++;
				}

				++ld.noOfBricks;
			}
		}
	}

	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			if (refBrickDataRef(row, col) != 255) spawnBrick(row, col);
		}
	}

	initLevelGfx();
	setupLevel();
}

void Game::restoreLevel() {
	preSetupLevel();
	initLevelGfx();

	// format brick data array
	for (unsigned char row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned char col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			refBrickDataRef(row, col) = 255; // reset
		}
	}

	unsigned char index = 0;

	// regenerate
	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			unsigned char &type = pCurrLevel->paBrickTypes[(row * BRICK_FIELD_BRICK_COLS) + col];
			if (type > 0) refBrickDataRef(row, col) = (index++) + 1;
		}
	}

	setupLevel();
}

void Game::countBrick(unsigned char row, unsigned char col) {
	// get brick data
	unsigned char bdref = refBrickDataRef(row, col);
	if (bdref == 255) return;

	BrickData &brickData = pLevelDatas[currLevelNo].brickData[bdref];
	unsigned const int type = brickData.type;

	brickData.active = false;
	//brickData.spriteID = 255;
	brickData.specialIndex = 0;

	if (type >= 20) {
		unsigned char realType = 255;
		unsigned char group = 255;

		GetGenericBrickGroupAndType(
			type,
			group,
			realType
		);
		pLevelDatas[currLevelNo].brickGroupData[group].activeBricks--;

		score += pCurrLevel->brickGroupInfos[group].points;

		if (brickGroupHit != 255) {
			// remove highlight
			drawChar(BOTTOM_SCREEN, 0, ' ', 27, 1 + brickGroupHit);
			brickGroupHit = 255; // reset
		}

		// add highlight
		drawChar(BOTTOM_SCREEN, 0, '>', 27, 1 + group, COLOUR_GREY);
		brickGroupHit = group; // reset
		brickGroupHitCounter = 60;

		if (pLevelDatas[currLevelNo].brickGroupData[group].activeBricks == 0) {
			resurrectBrickGroup(group);
		}

		updateScoreText();
	}
}

void Game::disposeLevel() { 

}

bool Game::testIntersectionWalls(
	int x1, int y1, int x2, int y2,
	unsigned char &side
) {
	side = RECT_SIDE_NONE;

	if (x1 < 0) {
		side = RECT_SIDE_LEFT;
		return true;
	}

	if (y1 < 0) {
		side = RECT_SIDE_TOP;
		return true;
	}

	if (x2 > (PLAY_FIELD_WIDTH - 1)) {
		side = RECT_SIDE_RIGHT;
		return true;
	}

	if (y2 > (PLAY_FIELD_HEIGHT - 1)) {
		side = RECT_SIDE_BOTTOM;
		return true;
	}

	return false;
}

bool Game::placeBall(
	const unsigned char ballIndex,
	const unsigned int brickRow,
	const unsigned int brickCol,
	const unsigned char side
) {
	Ball &ball = balls[0];

	if (side == RECT_SIDE_LEFT) {
		ball.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (brickCol * BRICK_WIDTH) - 1 - BALL_DIAMETER;
	} else if (side == RECT_SIDE_RIGHT) {
		ball.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + ((brickCol + 1) * BRICK_WIDTH);
	} else if (side == RECT_SIDE_TOP) {
		ball.y = (brickRow * BRICK_HEIGHT) - 1 - BALL_DIAMETER;
	} else if (side == RECT_SIDE_BOTTOM) {
		ball.y = ((brickRow + 1) * BRICK_HEIGHT);
	}
}

bool Game::findBrickSideHit(
	unsigned int row, unsigned int col,
	float ballX, float ballY,
	unsigned char &side
) {
	side = RECT_SIDE_NONE;

	RDSCircleF ballCl;
	ballCl.pos.x = ballX + (BALL_DIAMETER / 2);
	ballCl.pos.y = ballY + (BALL_DIAMETER / 2);
	ballCl.r = BALL_DIAMETER / 2;

	RDSLineF line;

	line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
	line.s.y = row * BRICK_HEIGHT;
	line.e.x = line.s.x + BRICK_WIDTH - 1;
	line.e.y = line.s.y;

	if (clIntersect(ballCl, line)) {
		side = RECT_SIDE_TOP;
		return true;
	}

	line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
	line.s.y = row * BRICK_HEIGHT;
	line.e.x = line.s.x;
	line.e.y = line.s.y + BRICK_HEIGHT - 1;

	if (clIntersect(ballCl, line)) {
		side = RECT_SIDE_RIGHT;
		return true;
	}

	line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
	line.s.y = ((row + 1) * BRICK_HEIGHT) - 1;
	line.e.x = line.s.x + BRICK_WIDTH - 1;
	line.e.y = line.s.y;

	if (clIntersect(ballCl, line)) {
		side = RECT_SIDE_BOTTOM;
		return true;
	}

	line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
	line.s.y = row * BRICK_HEIGHT;
	line.e.x = line.s.x;
	line.e.y = line.s.y + BRICK_HEIGHT - 1;

	if (clIntersect(ballCl, line)) {
		side = RECT_SIDE_LEFT;
		return true;
	}

	return false;
}

/*
bool Game::testIntersectionBricks(
	int x1, int y1, int x2, int y2,
	unsigned int &brickRow, unsigned int &brickCol,
	unsigned char &brickNo,
	unsigned char &side
) {
	side = RECT_SIDE_NONE;

	Ball &ball = balls[0];

	RDSCircleF ballCl;
	ballCl.pos.x = ball.x + (BALL_DIAMETER / 2);
	ballCl.pos.y = ball.y + (BALL_DIAMETER / 2);
	ballCl.r = BALL_DIAMETER / 2;

	RDSLineF line;

	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			unsigned char bdref = refBrickDataRef(row, col);
			if (bdref == 255) continue; // null reference

			BrickData &d = pLevelDatas[currLevelNo].brickData[bdref];
			if (d.active) {
				bool genericBrick = false;
				unsigned char realType = 255;
				unsigned char group = 255;

				if (d.type >= 20) {
					genericBrick = true;
					GetGenericBrickGroupAndType(
						d.type,
						group,
						realType
					);
				} else {
					realType = d.type;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
				line.s.y = row * BRICK_HEIGHT;
				line.e.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
				line.e.y = line.s.y;

				if (clIntersect(ballCl, line)) {
					side = RECT_SIDE_TOP;
					return true;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
				line.s.y = row * BRICK_HEIGHT;
				line.e.x = line.s.x;
				line.e.y = ((row + 1) * BRICK_HEIGHT) - 1;

				if (clIntersect(ballCl, line)) {
					side = RECT_SIDE_RIGHT;
					return true;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
				line.s.y = ((row + 1) * BRICK_HEIGHT) - 1;
				line.e.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
				line.e.y = line.s.y;

				if (clIntersect(ballCl, line)) {
					side = RECT_SIDE_BOTTOM;
					return true;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
				line.s.y = row * BRICK_HEIGHT;
				line.e.x = line.s.x;
				line.e.y = ((row + 1) * BRICK_HEIGHT) - 1;

				if (clIntersect(ballCl, line)) {
					side = RECT_SIDE_LEFT;
					return true;
				}
			}
		}
	} 
	return false;
}
*/

bool Game::testIntersectionBricks(
	float x1, float y1, float x2, float y2,
	unsigned char &brickRow, unsigned char &brickCol,
	unsigned char &brickNo,
	unsigned char &side
) {
	Ball &ball = balls[0];

	RDSRectI aoi;

	getFieldAreaIntersectRectLimited(
		x1, x2,
		y1, y2,
		aoi
	);

	side = RECT_SIDE_NONE;

	for (unsigned char row = aoi.y1; row <= aoi.y2; row++) {
		for (unsigned char col = aoi.x1; col <= aoi.x2; col++) {
			unsigned char bdref = refBrickDataRef(row, col);
			if (bdref == 255) continue; // null reference

			BrickData &d = pLevelDatas[currLevelNo].brickData[bdref];
			if (d.active) {
				bool genericBrick = false;
				unsigned char realType = 255;
				unsigned char group = 255;

				if (d.type >= 20) {
					genericBrick = true;
					GetGenericBrickGroupAndType(
						d.type,
						group,
						realType
					);
				} else {
					realType = d.type;
				}
				
				float
					brickX1 = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH),
					brickY1 = row * BRICK_HEIGHT,
					brickX2 = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1),
					brickY2 = ((row + 1) * BRICK_HEIGHT) - 1
				;

				if (rectIntersectsRectF(
					x1, y1, x2, y2,
					brickX1, brickY1,
					brickX2, brickY2
				)) {
					brickRow = row;
					brickCol = col;

					if ((x1 <= brickX1) && (x2 >= brickX1)) side = RECT_SIDE_LEFT;
					else if ((x1 <= brickX2) && (x2 >= brickX2)) side = RECT_SIDE_RIGHT;
					else if ((y1 <= brickY1) && (y2 >= brickY1)) side = RECT_SIDE_TOP;
					else if ((y1 <= brickY2) && (y2 >= brickY2)) side = RECT_SIDE_BOTTOM;

					if (side == RECT_SIDE_NONE) {
						findBrickSideHit(brickRow, brickCol, ball.x, ball.y, side);
					}
					
					if (side == RECT_SIDE_NONE) {
						side = RECT_SIDE_LEFT;
					}

					return true;
				}
			}
		}
	}

	/*
	unsigned char row, col, _side;

	bool intersecting = testBallIntersection(0, row, col, _side);
	if (intersecting) {
		unsigned char bdref = refBrickDataRef(row, col);
		if (bdref == 255) return false;

		BrickData &d = pLevelDatas[currLevelNo].brickData[bdref];
		if (d.active) {
			bool genericBrick = false;
			unsigned char realType = 255;
			unsigned char group = 255;

			if (d.type >= 20) {
				genericBrick = true;
				GetGenericBrickGroupAndType(
					d.type,
					group,
					realType
				);
			} else {
				realType = d.type;
			}

			brickRow = row;
			brickCol = col;

			side = _side;
			return true;
		}						
	}

	*/

	return false;
}

bool Game::testIntersectionBricks2(
	unsigned char ballNo,
	float x1, float y1,
	float x2, float y2,
	float vX, float vY,
	BrickCollisions &bcs
) {
	bcs.noOfCollisons = 0;
	Ball &ball = balls[ballNo];

	RDSCircleF ballCl;
	ballCl.pos.x = ball.x + (BALL_DIAMETER / 2);
	ballCl.pos.y = ball.y + (BALL_DIAMETER / 2);
	ballCl.r = BALL_DIAMETER / 2;

	RDSRectI aoi;
	RDSLineF line;
	getFieldAreaIntersectRectLimited(
		x1,
		x2,
		y1,
		y2,
		aoi
	);

	unsigned char currBrick = 0;
	for (unsigned char row = aoi.y1; row <= aoi.y2; row++) {
		for (unsigned char col = aoi.x1; col <= aoi.x2; col++) {
			if (refBrickDataRef(row, col) != 255) {
				BrickCollision &bc = bcs.collisions[currBrick];
				bc.row = row;
				bc.col = col;

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
				line.s.y = row * BRICK_HEIGHT;
				line.e.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
				line.e.y = line.s.y;

				bc.sidesHit[RECT_SIDE_TOP - 1] = clIntersect(ballCl, line);
				if (bc.sidesHit[RECT_SIDE_TOP - 1]) {
					bc.noOfSidesHit++;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
				line.s.y = row * BRICK_HEIGHT;
				line.e.x = line.s.x;
				line.e.y = ((row + 1) * BRICK_HEIGHT) - 1;

				bc.sidesHit[RECT_SIDE_RIGHT - 1] = clIntersect(ballCl, line);
				if (bc.sidesHit[RECT_SIDE_RIGHT - 1]) {
					bc.noOfSidesHit++;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
				line.s.y = ((row + 1) * BRICK_HEIGHT) - 1;
				line.e.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (((col + 1) * BRICK_WIDTH) - 1);
				line.e.y = line.s.y;

				bc.sidesHit[RECT_SIDE_BOTTOM - 1] = clIntersect(ballCl, line);
				if (bc.sidesHit[RECT_SIDE_BOTTOM - 1]) {
					bc.noOfSidesHit++;
				}

				line.s.x = BRICK_FIELD_X_PLAY_FIELD_OFFSET + (col * BRICK_WIDTH);
				line.s.y = row * BRICK_HEIGHT;
				line.e.x = line.s.x;
				line.e.y = ((row + 1) * BRICK_HEIGHT) - 1;

				bc.sidesHit[RECT_SIDE_LEFT - 1] = clIntersect(ballCl, line);
				if (bc.sidesHit[RECT_SIDE_LEFT - 1]) {
					bc.noOfSidesHit++;
				}

				if (bc.noOfSidesHit > 0) {
					bcs.noOfCollisons++;
					currBrick++;
				}
			}
		}
	}

	return bcs.noOfCollisons > 0;
}

void Game::getFieldAreaIntersectRectLimited(
	float x1, float x2,
	float y1, float y2,
	RDSRectI &fieldRect
) {
	fieldRect.x1 = (int)(x1 - BRICK_FIELD_X_PLAY_FIELD_OFFSET) / BRICK_WIDTH;
	fieldRect.x2 = (int)(x2 - BRICK_FIELD_X_PLAY_FIELD_OFFSET) / BRICK_WIDTH;

	fieldRect.y1 = (int)(y1 / BRICK_HEIGHT);
	fieldRect.y2 = (int)(y2 / BRICK_HEIGHT);
}

bool Game::testIntersectionBat(
	int x1, int y1,
	int x2, int y2
) {
	Bat &bat = bats[0];

	return rectIntersectsRectI(
		x1, y1,
		x2, y2,
		0, bat.topY,
		BAT_WIDTH - 1, bat.bottomY
	);
}

void Game::initForceGfx(bool removeOld) {
	drawText(BOTTOM_SCREEN, 0, "Force", FORCE_BIT_START_COL, FORCE_BIT_START_ROW, COLOUR_YELLOW, 0);
}

void Game::updateForceGfx(bool removeOld) {
	if (forceFieldLevel > 0) {
		bool field_odd = (forceFieldLevel % 2) == 1;
		char gfxIndex = 0;

		for (unsigned char i = 0; i < 10; i++) { 
			bool odd = (i % 2) == 1;
			if (!odd || i == forceFieldLevel) {
				if (i < forceFieldLevel) {
					drawCharRowCol(
						BOTTOM_SCREEN, 0, FORCE_BIT_START_COL + gfxIndex, FORCE_BIT_START_ROW + 1,
						24, (i == (forceFieldLevel - 1)) && field_odd ? 20 : 19
					);
				} else {
					if (removeOld) {
						drawChar(BOTTOM_SCREEN, 0, ' ', FORCE_BIT_START_COL + gfxIndex, FORCE_BIT_START_ROW + 1);
					}
				}
				gfxIndex++;
			}
		}
	} else {
		crazyForceUpdateTick = 100;
	}
}

void Game::drawChar(const u8 scr, const u8 bg, const char c, const s16 x, const s16 y, const unsigned char colour) {	
	if (c != ' ') {
		const char start = 32;
		//const unsigned short excessPerLine = 16;
		//const unsigned short charsPerLine = 16;
		const unsigned char charsPerLine = 32;
		char os = c - start;

		unsigned char row = (colour * 3) + os / charsPerLine;
		unsigned char col = os % charsPerLine;

		drawCharRowCol(scr, bg, x, y, row, col);
	} else {
		drawCharRowCol(scr, bg, x, y, 0, 0);
	}
}

void Game::drawText(const u8 scr, u8 bg, const char* text, const s16 x, const s16 y, const unsigned char colour, const unsigned char minWidth) {
	unsigned char len = strlen(text);
	unsigned char realWidth = MAX(minWidth, len);

	// init sprite stuff
	for (unsigned char i = 0; i < realWidth; i++) {
		if (i < len) {
			drawChar(scr, bg, text[i], x + i, y, colour);
		} else {
			drawChar(scr, bg, ' ', x + i, y, colour);
		}
	}
}

void Game::showMessage(const char* m_msg, const unsigned char colour) {
	strcpy(msg, m_msg);
	unsigned char newMsgLen = strlen(msg);
	msgTicks = 300;

	//drawText(TOP_SCREEN, 0, msg, MSG_TILE_START_COL, MSG_TILE_START_ROW);

	// init sprite stuff
	for (unsigned char i = 0; i < msgLen; i++) {
		drawChar(TOP_SCREEN, 0, ' ', MSG_TILE_START_COL + i, MSG_TILE_START_ROW);
	}

	for (unsigned char i = 0; i < newMsgLen; i++) {
		drawChar(TOP_SCREEN, 0, msg[i], MSG_TILE_START_COL + i, MSG_TILE_START_ROW);
	}

	msgLen = newMsgLen;
}

void Game::drawCharRowCol(const u8 scr, const u8 bg, const s16 x, const s16 y, const unsigned char row, const unsigned char col) {
	const unsigned char charsPerLine = 32;

	s16 tileNo = (
		(row * charsPerLine)
	) + col;
	
	PA_SetMapTile(scr, bg, x, y, tileNo);
}

void Game::update() {
	// fade out
	if (flashCounter > 0) {
		 PA_SetBrightness(1, flashCounter);
		 flashCounter -= 2;
	} else {			
		// Reset everything
		PA_SetBrightness(1, 0); // Back to normal
	}

	if (forceFieldLevel == 0 && ++crazyForceUpdateTick >= 2) {
		crazyForceUpdateTick = 0;
		crazyRand += vbl_count;

		for (unsigned char i = 0; i < 5; i++) {
			crazyRand += 32434;
			PA_SRand(crazyRand);
			unsigned char rand = PA_RandMax(6);
			drawCharRowCol(
				BOTTOM_SCREEN, 0, FORCE_BIT_START_COL + i, FORCE_BIT_START_ROW + 1,
				24, 13 + rand
			);
		}
	}

	if (msgTicks > 0) {
		if (--msgTicks == 0) {
			// remove message
			for (unsigned char i = 0; i < msgLen; i++) {
				// replace line characters with empty space -- should do the trick!
				drawChar(TOP_SCREEN, 0, ' ', MSG_TILE_START_COL + i, MSG_TILE_START_ROW);
			}
		}
	}

	if (brickGroupHitCounter > 0) {
		if (--brickGroupHitCounter == 0) {
			// remove highlight
			drawChar(BOTTOM_SCREEN, 0, ' ', 27, 1 + brickGroupHit);
			brickGroupHit = 255; // reset
		}
	}

	if (!running) return;

	if (Pad.Newpress.X) {
		if (ballsLeft > 0) {
			ballsLeft--;
			updateBallsGfx();
		}
		resetBall();
	}

	for (unsigned char i = 0; i < 2; i++) {
		// update game
		updateBats();
		updateBalls();
	}


	//char str[30];
	//sprintf(str, "BrickGFXCount: %d", brickGfxCount);
	//showDebugMsg(str, 4);

	//printDebugGroupData();

	fps++;

	u8 fpstime = PA_RTC.Seconds;
	
	if (lastFPSUpdateTime != fpstime) {
		if (fps > 99) fps = 99;
		updateFPSText(true);
		lastFPSUpdateTime = fpstime;
		fps = 0;
	}
}

//bool Game::getBrickSpriteInUse(u8 obj_number) {
//	return spriteUsed[obj_number];
//}

void Game::showDebugMsg(const char *pMsg, const unsigned char line) {
	drawText(BOTTOM_SCREEN, 0, pMsg, DEBUG_TEXT_START_COL, DEBUG_TEXT_START_ROW + line, COLOUR_GREY, 24);
}

void Game::updateBallsGfx(const bool removeOld) {
	unsigned char i = 0;
	for (; i < ballsLeft; i++) {
		drawCharRowCol(BOTTOM_SCREEN, 0, P1_TEXT_START_COL + i, P1_TEXT_START_ROW + 1, 24, 12);
	}
	if (removeOld) {
		for (; i < 5; i++) {
			drawChar(BOTTOM_SCREEN, 0, ' ', P1_TEXT_START_COL + i, P1_TEXT_START_ROW + 1);
		}
	}
}

void Game::preSetupLevel() {
}

void Game::setupLevel() {
	forceFieldLevel = 10;
	setupLevelGfx();
}

void Game::setupLevelGfx() {
	updateLevelNoText(true);
	updateForceGfx(true);
}

void SetupLevelData(LevelData &levelData) {
}

unsigned char& Game::refBrickDataRef(unsigned char row, unsigned char col) {
	return paBrickDataRef[(row * BRICK_FIELD_BRICK_COLS) + col];
}

#endif

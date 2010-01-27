#ifndef __main__
#define __main__

#include "main.h"

u32 vbl_count;  // Used to keep track of VBLs

// Tests for rect intersection.
bool rectIntersectsRectI(
	int aX1, int aY1, int aX2, int aY2, 
	int bX1, int bY1, int bX2, int bY2
) {
	return (
		((aX1 < bX2) && (aY1 < bY2) && (aX2 > bX1) && (aY2 > bY1)) ||
		((bX1 < aX2) && (bY1 < aY2) && (bX2 > aX1) && (bY2 > aY1))
	);
}

// Tests for rect intersection.
bool rectIntersectsRectF(
	float aX1, float aY1, float aX2, float aY2, 
	float bX1, float bY1, float bX2, float bY2
) {
	return (
		((aX1 < bX2) && (aY1 < bY2) && (aX2 > bX1) && (aY2 > bY1)) ||
		((bX1 < aX2) && (bY1 < aY2) && (bX2 > aX1) && (bY2 > aY1))
	);
}

bool lineIntersectsLineF(
	RDSLineF line1, RDSLineF line2,
	unsigned char &status,
	RDSPointF &intersection
) {
	status = LINES_NOT_INTERESECTING;

	float denom = ((line2.e.y - line2.s.y)*(line1.e.x - line1.s.x)) -
				  ((line2.e.x - line2.s.x)*(line1.e.y - line1.s.y));

	float nume_a = ((line2.e.x - line2.s.x)*(line1.s.y - line2.s.y)) -
				   ((line2.e.y - line2.s.y)*(line1.s.x - line2.s.x));

	float nume_b = ((line1.e.x - line1.s.x)*(line1.s.y - line2.s.y)) -
				   ((line1.e.y - line1.s.y)*(line1.s.x - line2.s.x));

	if (denom == 0.0f) {
		if(nume_a == 0.0f && nume_b == 0.0f) {
			status = LINES_COINCIDENT;
			return false;
		}
		status = LINES_PARALLEL;
		return false;
	}

	float ua = nume_a / denom;
	float ub = nume_b / denom;

	if(ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
	{
		// Get the intersection point.
		intersection.x = line1.s.x + ua*(line1.e.x - line1.s.x);
		intersection.y = line1.s.y + ua*(line1.e.y - line1.s.y);

		status = LINES_INTERESECTING;
		return true;
	}

	status = LINES_NOT_INTERESECTING;
	return false;
}

bool clIntersect(const RDSCircleF &c, const RDSLineF &l) {
	float x0 = c.pos.x;
	float y0 = c.pos.y;
	float x1 = l.s.x;
	float y1 = l.s.y;
	float x2 = l.e.x;
	float y2 = l.e.y;
	float n = Abs((x2-x1)*(y1-y0)-(x1-x0)*(y2-y1));
	float d = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	float dist = n/d;
	if(dist > c.r)return false;
	float d1 = sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
	if((d1-c.r) > d)return false;
	float d2 = sqrt((x0-x2)*(x0-x2)+(y0-y2)*(y0-y2));
	if((d2-c.r) > d)return false;
	return true;
}

bool getSpriteInUse(u8 screen, u8 obj_number) {
	return (
		(PA_obj[screen][obj_number].atr0 != 192) &&
		(PA_obj[screen][obj_number].atr1 != 256)
	);
}

bool getAvailableSprite(u8 screen, u8 start, u8 end) {
	for (u8 i = start; i <= end; i++) {
		if (!getSpriteInUse(screen, i)) return i;
	}
	return false;
}

s32 Modulo(s32 src, s32 divisor) {
   // Keep taking away until we can't take away no mores, and then return the result.    
   while (src >= divisor) src -= divisor;
   return src;
}

u32 intdiv;
u32 intdiv_returned;

u32 IntDiv(u32 src, u32 divisor) {
   // Keep adding until we have an equal to the rounded down answer.      
   intdiv = intdiv_returned = 0;

   src -= Modulo(src, divisor);

   while (intdiv != src) {
      intdiv += divisor;
      intdiv_returned++;
   }

   //intdiv_returned = 0;

   //while ((src -= divisor) != 0) {
   //   intdiv_returned++;
   //}

   return intdiv_returned;
}

s32 IntDivS(s32 src, s32 divisor) {                               
   if (!(src < 0) && !(divisor < 0)) return  IntDiv( src, divisor);
   if ( (src < 0) && !(divisor < 0)) return -IntDiv(-src, divisor);
   if (!(src < 0) &&  (divisor < 0)) return -IntDiv( src,-divisor);
   if ( (src < 0) &&  (divisor < 0)) return  IntDiv(-src,-divisor);

   return 0;
}

u32 IntDivR(u32 src, u32 divisor) {
   // Keep adding until we have an equal to the rounded down answer.
   intdiv = intdiv_returned = 0;   

   if (divisor != 1) {
      if ((Modulo(src, divisor) * 2) >= divisor) {
         intdiv_returned++;
      }
   }

   src -= Modulo(src, divisor);

   while (intdiv != src) {
      intdiv += divisor;
      intdiv_returned++;
   }

   //intdiv_returned = 0;

   //while ((src -= divisor) != 0) {
   //   intdiv_returned++;
   //}
   
   return intdiv_returned;
}

void ParseIntIntoComponents(u32 input, u32 *tens, u32 *units) {
   (*tens)  = IntDiv(input, 10);
   
   (*units) = Modulo(input, 10);   
}

u32 RangeWrap(s32 n, u32 greatest) {
   while (n < 0        ) n += greatest;
   while (n >= greatest) n -= greatest;
   
   return n;
}

int FloatRound(float a) {
   return ((int)(a + 0.5f)); 
}

int PowerOfTwo(int n) {
   int r = 1;
   for (;;) {      
      if (!n) return r;
      r <<= 1;   
      --n;
   }
}

int ClosestPowerOfTwo(int n) {
   int p = 2;
   for (;;) {
      if (n < (PowerOfTwo(p - 1) * 3)) return PowerOfTwo(p);   
      ++p;
   } 
}

// include cpp files ;)
#include "level.cpp"
#include "game.cpp"
#include "titlescr.cpp"
#include "hud.cpp"

GeneralBrickGroupInfo generalBrickGroupInfos[MAX_NO_OF_BRICK_GROUPS];

// code
int main(void)	{
	vbl_count = 0;

	// init palib
	PA_Init();
	PA_InitVBL();
	
	generalBrickGroupInfos[COLOUR_GREY] =
		GeneralBrickGroupInfo(1, "Grey", pal1_Pal, pal1_Pal8)
	;
	
	generalBrickGroupInfos[COLOUR_RED] =
		GeneralBrickGroupInfo(2, "Red", pal2_Pal, pal2_Pal8)
	;
	
	generalBrickGroupInfos[COLOUR_BLUE] =
		GeneralBrickGroupInfo(3, "Blue", pal3_Pal, pal3_Pal8)
	;
	
	generalBrickGroupInfos[COLOUR_DARK_BLUE] =
		GeneralBrickGroupInfo(4, "Dark Blue", pal4_Pal, pal4_Pal8)
	;
	
	generalBrickGroupInfos[COLOUR_LIGHT_BLUE] =
		GeneralBrickGroupInfo(5, "Light Blue", pal5_Pal, pal5_Pal8)
	;

	generalBrickGroupInfos[COLOUR_YELLOW] =
		GeneralBrickGroupInfo(6, "Yellow", pal6_Pal, pal6_Pal8)
	;

	generalBrickGroupInfos[COLOUR_GREEN] =
		GeneralBrickGroupInfo(7, "Green", pal7_Pal, pal7_Pal8)
	;

	generalBrickGroupInfos[COLOUR_ORANGE] =
		GeneralBrickGroupInfo(8, "Orange", pal8_Pal, pal8_Pal8)
	;

	Game game = Game();

	Level levels[3] = {
		{
			0,
			const_cast<unsigned char*>
			(DEFAULT_LEVELS_LEVEL_0_BRICK_TYPES),
			8,
			{
				LevelBrickGroupInfo(COLOUR_GREY, 5),
				LevelBrickGroupInfo(COLOUR_RED, 10),
				LevelBrickGroupInfo(COLOUR_BLUE, 15),
				LevelBrickGroupInfo(COLOUR_DARK_BLUE, 20),
				LevelBrickGroupInfo(COLOUR_LIGHT_BLUE, 50),
				LevelBrickGroupInfo(COLOUR_GREEN, 75),
				LevelBrickGroupInfo(COLOUR_ORANGE, 150),
				LevelBrickGroupInfo(COLOUR_YELLOW, 300)
			}
		}, {
			1,
			const_cast<unsigned char*>
			(DEFAULT_LEVELS_LEVEL_1_BRICK_TYPES),
			8,
			{
				LevelBrickGroupInfo(COLOUR_GREY, 10),
				LevelBrickGroupInfo(COLOUR_RED, 15),
				LevelBrickGroupInfo(COLOUR_BLUE, 20),
				LevelBrickGroupInfo(COLOUR_DARK_BLUE, 30),
				LevelBrickGroupInfo(COLOUR_LIGHT_BLUE, 60),
				LevelBrickGroupInfo(COLOUR_GREEN, 80),
				LevelBrickGroupInfo(COLOUR_ORANGE, 130),
				LevelBrickGroupInfo(COLOUR_YELLOW, 150)
			}
		}, {
			2,
			const_cast<unsigned char*>
			(DEFAULT_LEVELS_LEVEL_2_BRICK_TYPES),
			8,
			{
				LevelBrickGroupInfo(COLOUR_GREY, 5),
				LevelBrickGroupInfo(COLOUR_RED, 10),
				LevelBrickGroupInfo(COLOUR_BLUE, 20),
				LevelBrickGroupInfo(COLOUR_DARK_BLUE, 30),
				LevelBrickGroupInfo(COLOUR_LIGHT_BLUE, 60),
				LevelBrickGroupInfo(COLOUR_GREEN, 80),
				LevelBrickGroupInfo(COLOUR_ORANGE, 130),
				LevelBrickGroupInfo(COLOUR_YELLOW, 150)
			}
		}
	};

	SetupLevel(levels[0]);
	SetupLevel(levels[1]);
	SetupLevel(levels[2]);

	LevelData levelDatas[3];
	levelDatas[0].inUse = false;
	levelDatas[1].inUse = false;
	levelDatas[2].inUse = false;

	LevelSet defaultLevelSet = {
		"Dess1", "Dessimat0r",
		BRICK_FIELD_BRICK_ROWS,
		BRICK_FIELD_BRICK_COLS,
		levels, 3
	};

	PA_SetLedBlink(0, 0);
	PA_InitRand();

	PA_SetBrightness(0, 31);
	PA_SetBrightness(1, 31);
	
	s16 scroll0 = 0;
	s16 scroll1 = 256;

	PA_LoadTiledBg(1, 3, logo1);
	
	s8 i;
	for (i = 31; i >= 0; i--) {
	   PA_SetBrightness(0, i);
	   PA_SetBrightness(1, i);	   
	   PA_WaitForVBL();
	}

	// Infinite loop to keep the program running
	while ((!Pad.Newpress.Anykey)&&(!Stylus.Newpress)) {
		PA_WaitForVBL();
	}

	// fade out
	for (i = 0; i < 31; i++) {
		 PA_SetBrightness(0, i);
		 PA_SetBrightness(1, i);	   
		 PA_WaitForVBL();
	}
		
	// Reset everything
	PA_ResetSpriteSys(); 
	PA_ResetBgSys(); 
	PA_SetBrightness(0, 0); // Bacl to normal
	PA_SetBrightness(1, 0); // Back to normal

	game = Game(&defaultLevelSet, levelDatas);

	while(1) {
		if (game.inited) game.update();
		PA_WaitForVBL();

		++vbl_count;
	}
	
	return 0;
}

#endif

#ifdef __main__
#include "level.h"

void GetGenericBrickGroupAndType(
	const unsigned char &type,
	unsigned char &ggroup,
	unsigned char &gtype
) {
	gtype = type - 20; // generic bricks start at 20 in groups of 10
	ggroup = gtype / 10; // obtain group
	gtype = gtype % 10;	 // obtain type
}

void SetupLevel(Level &level) {
	for (unsigned int row = 0; row < BRICK_FIELD_BRICK_ROWS; row++) {
		for (unsigned int col = 0; col < BRICK_FIELD_BRICK_COLS; col++) {
			const unsigned char &type =
				level.paBrickTypes[(row * BRICK_FIELD_BRICK_COLS) + col]
			;
			if (type >= 20) {
				unsigned char ggroup;
				unsigned char gtype;

				GetGenericBrickGroupAndType(type, ggroup, gtype);

				level.brickGroupInfos[ggroup].bricks++;
			}
		}
	}
}

#endif

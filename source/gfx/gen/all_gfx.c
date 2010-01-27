//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Sprite files : 
#include "bat.c"
#include "ball.c"
#include "brick1.c"
#include "brick2.c"
#include "gem1.c"
#include "brick1blue2.c"
#include "brick1green.c"
#include "brick1orange.c"
#include "brick1yellow.c"

// Background files : 
#include "font.c"
#include "logo1.c"
#include "bg1.c"
#include "bricks.c"

// Palette files : 
#include "bat.pal.c"
#include "ball.pal.c"
#include "brick1.pal.c"
#include "brick2.pal.c"
#include "gem1.pal.c"
#include "brick1blue2.pal.c"
#include "brick1green.pal.c"
#include "brick1orange.pal.c"
#include "brick1yellow.pal.c"
#include "font.pal.c"
#include "logo1.pal.c"
#include "bg1.pal.c"
#include "bricks.pal.c"

// Background Pointers :
PAGfx_struct font = {(void*)font_Map, 1024, (void*)font_Tiles, 50496, (void*)font_Pal, (int*)font_Info };
PAGfx_struct logo1 = {(void*)logo1_Map, 768, (void*)logo1_Tiles, 15232, (void*)logo1_Pal, (int*)logo1_Info };
PAGfx_struct bg1 = {(void*)bg1_Map, 768, (void*)bg1_Tiles, 49088, (void*)bg1_Pal, (int*)bg1_Info };


#ifdef __cplusplus
}
#endif


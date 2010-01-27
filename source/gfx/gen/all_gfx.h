//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char bat_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : bat_Pal
extern const unsigned char ball_Sprite[32] __attribute__ ((aligned (4))) ;  // Pal : ball_Pal
extern const unsigned char brick1_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : brick1_Pal
extern const unsigned char brick2_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : brick2_Pal
extern const unsigned char gem1_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : gem1_Pal
extern const unsigned char brick1blue2_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : brick1blue2_Pal
extern const unsigned char brick1green_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : brick1green_Pal
extern const unsigned char brick1orange_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : brick1orange_Pal
extern const unsigned char brick1yellow_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : brick1yellow_Pal

// Background files : 
extern const int font_Info[3]; // BgMode, Width, Height
extern const unsigned short font_Map[1024] __attribute__ ((aligned (4))) ;  // Pal : font_Pal
extern const unsigned char font_Tiles[50496] __attribute__ ((aligned (4))) ;  // Pal : font_Pal
extern PAGfx_struct font; // background pointer

extern const int logo1_Info[3]; // BgMode, Width, Height
extern const unsigned short logo1_Map[768] __attribute__ ((aligned (4))) ;  // Pal : logo1_Pal
extern const unsigned char logo1_Tiles[15232] __attribute__ ((aligned (4))) ;  // Pal : logo1_Pal
extern PAGfx_struct logo1; // background pointer

extern const int bg1_Info[3]; // BgMode, Width, Height
extern const unsigned short bg1_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg1_Pal
extern const unsigned char bg1_Tiles[49088] __attribute__ ((aligned (4))) ;  // Pal : bg1_Pal
extern PAGfx_struct bg1; // background pointer

extern const unsigned char bricks_Bitmap[1024] __attribute__ ((aligned (4))) ;  // Pal : bricks_Pal


// Palette files : 
extern const unsigned short bat_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short ball_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short brick1_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short brick2_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short gem1_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short brick1blue2_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short brick1green_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short brick1orange_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short brick1yellow_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short font_Pal[41] __attribute__ ((aligned (4))) ;
extern const unsigned short logo1_Pal[246] __attribute__ ((aligned (4))) ;
extern const unsigned short bg1_Pal[63] __attribute__ ((aligned (4))) ;
extern const unsigned short bricks_Pal[4] __attribute__ ((aligned (4))) ;


#endif


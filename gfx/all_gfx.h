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
extern const unsigned char bat_Sprite[256] __attribute__ ((aligned (4))) ;  // Pal : bat_Pal
extern const unsigned char ball_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : ball_Pal
extern const unsigned char brick1_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : brick1_Pal
extern const unsigned char brick2_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : brick2_Pal

// Palette files : 
extern const unsigned short bat_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short ball_Pal[3] __attribute__ ((aligned (4))) ;
extern const unsigned short brick1_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short brick2_Pal[4] __attribute__ ((aligned (4))) ;


#endif


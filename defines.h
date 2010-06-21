#ifndef DEFINES_H__
#define DEFINES_H__

#define WIDTH 800
#define HEIGHT 600

#define TILE 20

#define GRID_COLOR 0xaaaaff33

#define HEAD_COLOR 0xff0000ff
#define BODY_COLOR 0xff6600ff

#define LEFT   direction(-1,0)
#define RIGHT  direction(1,0)
#define UP     direction(0,-1)
#define DOWN   direction(0,1)
#define NOWHERE direction(0,0)
#define TERMINATE direction(1,1)

#endif

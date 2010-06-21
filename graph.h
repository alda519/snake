#ifndef GRAPH_H__
#define GRAPH_H__ 

int drawGrid(SDL_Surface *);

int clearTile(SDL_Surface *, TSnakeSegment);

//int drawSnakeSegment(SDL_Surface *, TSnakeSegment, int color);
int drawSnakeSegment(SDL_Surface *, SDL_Surface *, TSnakeSegment, int);

int drawTile(SDL_Surface *, TSnakeSegment);

#endif

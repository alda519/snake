#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "defines.h"
#include "game.h"

int drawTile(SDL_Surface *screen, TSnakeSegment snake)
{
  filledCircleColor(screen, snake.x*TILE+TILE/2, snake.y*TILE+TILE/2, TILE/2-3, 0x0000ffff);
  SDL_UpdateRect(screen, snake.x*TILE, snake.y*TILE, TILE, TILE);
  return 1;
}

int drawBoard(SDL_Surface *screen, SDL_Surface *data, TBoard board)
{
  SDL_Rect tile = {.y = 0, .w = TILE, .h = TILE};
  SDL_Rect desk =  {.w = TILE, .h = TILE};
  
  for(int i = 0; i < board.h; ++i) {
    for(int j = 0; j < board.w; ++j) {
      //tile.x = board[i][j]TILE TODO TODO
      SDL_BlitSurface(data, &tile, screen, &desk);
    }
  }
  return 0;
}

int clearTile(SDL_Surface *screen, TSnakeSegment snake)
{
  SDL_Rect r = { .w = TILE, .h = TILE, .x = snake.x*TILE , .y = snake.y*TILE };
  SDL_FillRect(screen, &r, 0);
  SDL_UpdateRect(screen, r.x,r.y,r.w,r.h);
  return 1;
}

int drawSnakeSegment(SDL_Surface *screen, SDL_Surface *data, TSnakeSegment snake, int color)
{
  clearTile(screen, snake);

  SDL_Rect obrhad = {.x = snake.image*TILE, .y = 0, .w = TILE, .h = TILE};
  SDL_Rect obraz =  {.x = snake.x*TILE, .y = snake.y*TILE, .w = TILE, .h = TILE};

  if(snake.type == EATENFOOD)
    obrhad.y = TILE;
  
  SDL_BlitSurface(data, &obrhad, screen, &obraz);

  SDL_UpdateRect(screen, snake.x*TILE,snake.y*TILE,TILE,TILE);
  return 1;
}

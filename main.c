#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>

#include "defines.h"
#include "game.h"
#include "graph.h"

#define TICK_INTERVAL 90

static int next_time;

int main(int argc, char *argv[])
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Surface *screen;
  SDL_putenv("SDL_VIDEO_CENTERED=1");
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE);
  if(screen == NULL) {
    fprintf(stderr, "SDL: %s\n", SDL_GetError());
    return 1;
  }
  SDL_WM_SetCaption("Snake", NULL);

  SDL_Surface *pics = SDL_LoadBMP("snake.bmp");
  if(pics == NULL) {
    // TODO freeall
    fprintf(stderr, "datovy soubor %s nenalezen\n", "snake.bmp");
    return 1;
  }
  SDL_SetColorKey(pics, SDL_SRCCOLORKEY, 0);

  srand(time(NULL));

  TSnake snake;
  TBoard board;
  TDirection dirct, tmpd;
  TSnakeSegment food = {.type = FOOD};

  int status = OK;

  while(status != EXIT) {
    snake = newSnake(WIDTH/TILE/2, HEIGHT/TILE/2);
    board = newBoard(WIDTH/TILE, HEIGHT/TILE);
    if(board.b == NULL)
      return 1;
    if(snake.head == NULL)
      return 1;
    SDL_FillRect(screen, NULL, 0);
    drawSnakeSegment(screen, pics, *snake.head, HEAD_COLOR);
    SDL_UpdateRect(screen, 0,0,0,0);
    do {
      food.x = rand()%(WIDTH/TILE);
      food.y = rand()%(HEIGHT/TILE);
    } while(board.b[food.y][food.x] != NIL);
    board.b[food.y][food.x] = FOOD;
    drawTile(screen, food);

    do {
      dirct = tmpd = getDirection(NOWHERE, NOWHERE);
      SDL_Delay(10);
    } while(dirct.dx == 0 && dirct.dy == 0);
    
    if(dirct.dx && dirct.dy)
      status = EXIT;
    else
      status = OK;
    
    
    next_time = SDL_GetTicks() + TICK_INTERVAL;

    while(status == OK) {
      status =  moveSnake(screen, pics, board, &snake, dirct); // EDITNG
      if(status == EATING) {
        do {
          food.x = rand()%(WIDTH/TILE);
          food.y = rand()%(HEIGHT/TILE);
        } while(board.b[food.y][food.x] != NIL);
        board.b[food.y][food.x] = FOOD;
        drawTile(screen, food);
        status = OK;
      }

      while(next_time > SDL_GetTicks()) {
        tmpd = getDirection(dirct, tmpd);
        SDL_Delay(10);
      }
      if(tmpd.dx == tmpd.dy)
        status = EXIT;
      dirct = tmpd;
      next_time = SDL_GetTicks() + TICK_INTERVAL;  //next_time += TICK_INTERVAL;
    }

    deleteSnake(snake);
    deleteBoard(board);
  }

  SDL_FreeSurface(pics);
  SDL_FreeSurface(screen);
  SDL_Quit();
  return 0;
}

/* TODO:
    nacist druhy datak
    vymyslet kresleni zdi
*/

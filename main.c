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

  SDL_Surface *picSnake = SDL_LoadBMP("snake.bmp");
  if(picSnake == NULL) {
    fprintf(stderr, "datovy soubor %s nenalezen\n", "snake.bmp");
    return 1;
  }

  SDL_Surface *picTiles = SDL_LoadBMP("board.bmp");
  if(picTiles == NULL) {
    fprintf(stderr, "datovy soubor %s nenalezen\n", "board.bmp");
    return 1;
  }
  SDL_SetColorKey(picTiles, SDL_SRCCOLORKEY, 0);
  
  SDL_Surface *picFood = SDL_LoadBMP("food.bmp");
  if(picFood == NULL) {
    fprintf(stderr, "datovy soubor %s nenalezen\n", "food.bmp");
    return 1;
  }

  srand(time(NULL));

  TSnake snake;
  TBoard board;
  TDirection dirct, tmpd;
  TSnakeSegment food = {.type = FOOD};

  int status = OK;

  while(status != EXIT) {
    snake = newSnake(WIDTH/TILE/2, HEIGHT/TILE/2);
    if(snake.head == NULL)
      return 1; // TODO lip uklidit

    board = newBoard(WIDTH/TILE, HEIGHT/TILE);
    if(board.b == NULL)
      return 1; // TODO: lip uklidit
    loadBoard(board, "level0.map");
    SDL_FillRect(screen, NULL, 0);
    drawBoard(screen, picTiles, board);

    drawSnakeSegment(screen, picSnake, *snake.head, HEAD_COLOR);
    SDL_UpdateRect(screen, 0,0,0,0);
    do {
      food.x = rand()%(WIDTH/TILE);
      food.y = rand()%(HEIGHT/TILE);
    } while(board.b[food.y][food.x] != NIL);
    board.b[food.y][food.x] = FOOD;
    drawTile(screen, picFood, food);

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
      status =  moveSnake(screen, picSnake, board, &snake, dirct);
      if(status == EATING) {
        do {
          food.x = rand()%(WIDTH/TILE);
          food.y = rand()%(HEIGHT/TILE);
        } while(board.b[food.y][food.x] != NIL);
        board.b[food.y][food.x] = FOOD;
        drawTile(screen, picFood, food);
        status = OK;
      }

      while(next_time > SDL_GetTicks()) {
        tmpd = getDirection(dirct, tmpd);
        SDL_Delay(10);
      }
      if(tmpd.dx == tmpd.dy)
        status = EXIT;
      dirct = tmpd;
      next_time = SDL_GetTicks() + TICK_INTERVAL;
    }

    deleteSnake(snake);
    deleteBoard(board);
  }

  SDL_FreeSurface(picSnake);
  SDL_FreeSurface(picTiles);
  SDL_FreeSurface(picFood);
  SDL_FreeSurface(screen);
  SDL_Quit();
  return 0;
}

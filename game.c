#include <stdlib.h>
#include <SDL/SDL.h>

#include "game.h"
#include "defines.h"

#include "graph.h"

TDirection direction(int dx, int dy)
{
  TDirection d = { .dx = dx, .dy = dy };
  return d;
}

// *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***

TSnake newSnake(int x, int y)
{
  TSnake s;
  
  s.head = malloc(sizeof(TSnakeSegment));
  s.tail = s.head;
  
  if(s.head == NULL)
    return s;

  s.head->x = x;
  s.head->y = y;
  s.head->type = EATENFOOD; // hned po prvnim kroku povyroste a prestane bugovat fronta
  s.head->image = HEAD; //TAIL_L;
  s.head->next = NULL;
  return s;
}

TBoard newBoard(int w, int h)
{
  TBoard b = {.w = w, .h = h};
  b.b = malloc(sizeof(int *) * h);
  if(b.b == NULL)
    return  b;

  for(int i = 0; i < h; ++i) {
    b.b[i] = malloc(sizeof(int) * w);
    if(b.b[i] == NULL) {
      for(--i; i >= 0; --i) {
        free(b.b[i]);
      }
      free(b.b);
      b.b = NULL;
      return b;
    }
    for(int j = 0; j < w; ++j) {
      //if(j == 0 || j + 1 == w || i == 0 || i + 1 == h)
      //  b.b[i][j] = WALL;
      //else
        b.b[i][j] = NIL;
    }
  }
  return b;
}

// *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***

int loadBoard(TBoard board, char *filename)
{
  int c;
  FILE *f = fopen(filename, "r");
  if(f == NULL)
    return 0;
  for(int i = 0; i < board.h; ++i) {
    for(int j = 0; j < board.w; ++j) {
      c = fgetc(f);
      if(c == EOF) {
        fclose(f);
        return 0;
      }
      if(c == '\n')
        continue;
      else if(c == '#')
        board.b[i][j] = WALL;
      else
        board.b[i][j] = NIL;
    }
  }

  fclose(f);
  return 1;
}

// *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***

int deleteSnake(TSnake snake)
{
  TSnakeSegment *s;
  while(snake.tail != NULL) {
    s = snake.tail;
    snake.tail = snake.tail->next;
    free(s);
  }
  return 1;
}

int deleteBoard(TBoard board)
{
  for(int i = 0; i < board.h; ++i)
    free(board.b[i]);
  free(board.b);
  return 1;
}

// *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***

TDirection getDirection(TDirection dirct, TDirection def)
{
  SDL_Event event;
  Uint8 *keys;

  if(!SDL_PollEvent(&event))
    return def;

  keys = SDL_GetKeyState(NULL);
  if(event.type == SDL_QUIT || (keys[SDLK_F4] && keys[SDLK_LALT]))
    return TERMINATE;
  if(keys[SDLK_RETURN] || keys[SDLK_ESCAPE])
    return TERMINATE;
  if(keys[SDLK_RIGHT] && dirct.dx == 0)
    return RIGHT;
  if(keys[SDLK_LEFT] && dirct.dx == 0)
    return LEFT;
  if(keys[SDLK_UP] && dirct.dy == 0)
    return UP;
  if(keys[SDLK_DOWN] && dirct.dy == 0)
    return DOWN;
    
  return def;
}

// *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***

//  TAIL (out) [>] .. [>] .. [>] .. [X]  HEAD (in)

int moveSnake(SDL_Surface *screen, SDL_Surface *pics, TBoard board, TSnake *snake, TDirection dir) // EDITNG
{
  TSnakeSegment *s;

  if(snake->tail->type == SNAKE) {
    clearTile(screen, *snake->tail);
    s = snake->tail;
    board.b[s->y][s->x] = NIL;
    snake->tail = snake->tail->next;
    
    if(snake->tail->next == NULL) {
      if(dir.dx == 1)
        snake->tail->image = TAIL_L;
      else if(dir.dx == -1)
        snake->tail->image = TAIL_R;
      else if(dir.dy == 1)
        snake->tail->image = TAIL_U;
      else
        snake->tail->image = TAIL_D;
    } else if(snake->tail->next->x - snake->tail->x == 1 || snake->tail->next->x - snake->tail->x < -1) {
      snake->tail->image = TAIL_L;
    } else if(snake->tail->next->x - snake->tail->x == -1 || snake->tail->next->x - snake->tail->x > 1) {
      snake->tail->image = TAIL_R;
    } else if(snake->tail->next->y - snake->tail->y == 1 || snake->tail->next->y - snake->tail->y < -1) {
      snake->tail->image = TAIL_U;
    } else if(snake->tail->next->y - snake->tail->y == -1 || snake->tail->next->y - snake->tail->y > 1) {
      snake->tail->image = TAIL_D;
    }
    drawSnakeSegment(screen, pics, *snake->tail, HEAD_COLOR); // EDITNG
  } else if(snake->tail->type == EATENFOOD) {
    s = malloc(sizeof(TSnakeSegment)); // konec neni posunut, protoze je tam jidlo
    snake->tail->type = SNAKE;
    s->type = SNAKE;
    s->next = NULL;
    if(snake->head == snake->tail){
      if(dir.dx == 1)
        snake->tail->image = TAIL_L;
      else if(dir.dx == -1)
        snake->tail->image = TAIL_R;
      else if(dir.dy == 1)
        snake->tail->image = TAIL_U;
      else if(dir.dy == -1)
        snake->tail->image = TAIL_D;
      drawSnakeSegment(screen, pics, *snake->tail, BODY_COLOR);
    }
  } else {
    return ERROR;
  }

  /*if(snake->head == snake->tail && snake->tail->next == snake->head) { SNAHA UDELAT PRVNI KULI KULATOU
    s->image = HEAD;
  } else*/ if(dir.dx == 1) {  // nastaveni tvaru hlavy
    s->image = HEAD_R;
  } else if(dir.dx == -1) {
    s->image = HEAD_L;
  } else if(dir.dy == 1) {
    s->image = HEAD_D;
  } else if(dir.dy == -1) {
    s->image = HEAD_U;
  }

  if     (dir.dx ==  1 && (snake->head->image == HEAD_D)) // predelani minule hlavy
    snake->head->image = BODY_RU;
  else if(dir.dx ==  1 && (snake->head->image == HEAD_U))
    snake->head->image = BODY_RD;
  else if(dir.dx == -1 && (snake->head->image == HEAD_D))
    snake->head->image = BODY_LU;
  else if(dir.dx == -1 && (snake->head->image == HEAD_U))
    snake->head->image = BODY_LD;
  else if(dir.dy ==  1 && (snake->head->image == HEAD_L))
    snake->head->image = BODY_RD;
  else if(dir.dy ==  1 && (snake->head->image == HEAD_R))
    snake->head->image = BODY_LD;
  else if(dir.dy == -1 && (snake->head->image == HEAD_L))
    snake->head->image = BODY_RU;
  else if(dir.dy == -1 && (snake->head->image == HEAD_R))
    snake->head->image = BODY_LU;
  else if((dir.dx ==  1 || dir.dx == -1) && (snake->head->image == HEAD_R || snake->head->image == HEAD_L))
    snake->head->image = BODY_H;
  else
    snake->head->image = BODY_V;
  if (snake->head != snake->tail)
    drawSnakeSegment(screen, pics, *snake->head, BODY_COLOR); // EDITNG

  s->x = snake->head->x + dir.dx;  // zmena souradnic
  s->y = snake->head->y + dir.dy;   
  
  if(s->x < 0)              // pretekani pres pole :)
    s->x = WIDTH/TILE-1;    //
  if(s->y < 0)              //
    s->y = HEIGHT/TILE-1;   //
  if(s->x >= WIDTH/TILE)    //
    s->x = 0;               //
  if(s->y >= HEIGHT/TILE)   //
    s->y = 0;               //

  snake->head->next = s;            //
  snake->head = s;             // pridani na hlavicku
  snake->head->next = NULL;
  
  switch(board.b[s->y][s->x]) {
    case NIL:
      snake->head->type = SNAKE;
      board.b[s->y][s->x] = SNAKE;
      drawSnakeSegment(screen, pics, *s, HEAD_COLOR); // EDITNG
      //SDL_UpdateRect(screen, 0,0,0,0);
      return OK;
      break;
    case FOOD:
      snake->head->type = EATENFOOD;
      board.b[s->y][s->x] = EATENFOOD;
      drawSnakeSegment(screen, pics, *s, HEAD_COLOR); // EDITNG
      //SDL_UpdateRect(screen, 0,0,0,0);
      return EATING;
      break;
    case SNAKE:
    case EATENFOOD:
    case WALL:
      return COLLISION;
      break;
    default:
      return ERROR;
  }
}

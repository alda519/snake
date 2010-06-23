#ifndef GAME_H__
#define GAME_H__

typedef enum {
  NIL, SNAKE, FOOD, EATENFOOD, WALL
} ETile;

typedef enum {
  OK, COLLISION, EATING, ERROR, EXIT
} EAction;

typedef enum {
  BODY_V, BODY_H, BODY_LU, BODY_LD, BODY_RU, BODY_RD,
  TAIL_L, TAIL_R, TAIL_U, TAIL_D,
  HEAD_L, HEAD_R, HEAD_U, HEAD_D, HEAD
} ESnake;

typedef enum {
  TILE_NIL, TILE_L, TILE_U, TILE_R, TILE_D,
  TILE_CLU, TILE_CLD, TILE_CRU, TILE_CRD
} EWall;

typedef struct t_snake_segment {
  int x, y;
  int type;
  int image;
  struct t_snake_segment *next;
} TSnakeSegment;

typedef struct {
  TSnakeSegment *head, *tail;
} TSnake;

typedef struct {
  int dx, dy;
} TDirection;

typedef struct {
  int w, h;
  int **b;
} TBoard;

TDirection direction(int, int);

TSnake newSnake(int, int);
TBoard newBoard(int, int);

int deleteBoard(TBoard);
int deleteSnake(TSnake);

int moveSnake(SDL_Surface *, SDL_Surface *, TBoard, TSnake*, TDirection); // EDITNG

TDirection getDirection(TDirection, TDirection);

#endif

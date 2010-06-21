snake: main.c game.c graph.c
	gcc -std=c99 -pedantic -Wall -g main.c game.c graph.c -o snake -lSDL -lSDL_gfx

PROJ=snake

OPTS=-std=c99 -pedantic -Wall -g
LIBS=-lSDL

OBJT=main.o game.o graph.o
HEAD=defines.h game.h graph.h

$(PROJ): $(OBJT) $(HEAD)
	gcc $(OPTS) $(LIBS) $(OBJT) -o $(PROJ)

main.o: main.c $(HEAD)
	gcc $(OPTS) -c main.c
game.o: game.c $(HEAD)
	gcc $(OPTS) -c game.c
graph.o: graph.c $(HEAD)
	gcc $(OPTS) -c graph.c

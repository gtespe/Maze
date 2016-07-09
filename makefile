all: maze.o maze

maze.o: maze.c
	gcc -c maze.c

maze: maze.o
	gcc maze.o -o maze


CC = gcc
FLAGS = -Wall -g

all: graph

graph: main.o algo.o graph.o
	$(CC) $(FLAGS) -o graph main.o algo.o graph.o

main.o: main.c graph.h
	$(CC) $(FLAGS) -c main.c

algo.o: algo.c graph.h
	$(CC) $(FLAGS) -c algo.c

graph.o: graph.c graph.h
	$(CC) $(FLAGS) -c graph.c

.PHONY: clean all
clean:
	rm -f *.o graph

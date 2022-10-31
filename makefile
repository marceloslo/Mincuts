CC = g++
CFLAGS = -Wall -Wextra

all: main

main: graph.o binheap.o  main.o  
	$(CC) $(CFLAGS) -o main main.o binheap.o graph.o headers/fibonacciheap.hpp

binheap.o: sources/binheap.cpp headers/binheap.hpp
	$(CC) -c sources/binheap.cpp -o binheap.o 

graph.o: sources/Graph.cpp headers/Graph.hpp
	$(CC) -c sources/Graph.cpp -o graph.o 

main.o: main.cpp
	$(CC) -c main.cpp -o main.o 
	
clean:
  ifeq ($(OS),Windows_NT)
	del main.o binheap.o graph.o
  else
	rm main.o binheap.o graph.o
  endif
main: main.o list.o
	gcc -Wall -g main.o list.o -o main
list.o: list.h list.c
	gcc -c list.c list.h
main.o: main.c
	gcc -c main.c -o main.o

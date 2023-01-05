all: client server clean
client: client.o
	gcc -Wall -g client.o -o client
client.o: client.c
	gcc -c client.c -o client.o
server: server.o
	gcc -Wall -g server.o -o server -pthread
server.o: server.c
	gcc -c server.c -o server.o
clean:
	rm -rf *.o
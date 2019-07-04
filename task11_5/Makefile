length: length.c
	gcc -Wall -g length.c -o length
length.c: length.y
	yacc -o length.c length.y
clean:
	rm -rf *.o
run:
	./length

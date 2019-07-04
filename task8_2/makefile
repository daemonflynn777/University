CC = g++
PROG = matr
CFLAGS = -g -Wall

$(PROG): main.cpp matrix.o
	$(CC) $(CFLAGS) main.cpp matrix.o -o $(PROG)
	$(CC) $(CFLAGS) tests.cpp matrix.o -o tests
matrix.o: matrix.cpp matrix.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(PROG) tests
run:
	./$(PROG)
testing:
	 ./tests
valgrind:
	valgrind --tool=memcheck ./tests

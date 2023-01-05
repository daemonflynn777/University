CC = g++
PROG = figure
CFLAGS = -g -Wall

$(PROG): main.cpp base.o circle.o square.o squarecut.o
	$(CC) $(CFLAGS) main.cpp base.o circle.o square.o squarecut.o -o $(PROG)
base.o: base.cpp base.h
	$(CC) $(CFLAGS) -c $< -o $@
circle.o: circle.cpp circle.h
	$(CC) $(CFLAGS) -c $< -o $@
square.o: square.cpp square.h
	$(CC) $(CFLAGS) -c $< -o $@
squarecut.o: squarecut.cpp squarecut.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(PROG) tests
run:
	./$(PROG)
valgrind:
	valgrind --tool=memcheck ./$(PROG)

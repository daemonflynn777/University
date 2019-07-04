CC = g++
PROG = lang
CFLAGS = -g -Wall

$(PROG): main.cpp error.o execution.o id.o tokenizing.o lexem.o RPN.o parser.o table.o
	$(CC) $(CFLAGS) main.cpp error.o execution.o id.o tokenizing.o lexem.o RPN.o parser.o table.o -o $(PROG)
error.o: error.cpp error.h
	$(CC) $(CFLAGS) -c $< -o $@
execution.o: execution.cpp execution.h
	$(CC) $(CFLAGS) -c $< -o $@
id.o: id.cpp id.h
	$(CC) $(CFLAGS) -c $< -o $@
tokenizing.o: tokenizing.cpp tokenizing.h
	$(CC) $(CFLAGS) -c $< -o $@
lexem.o: lexem.cpp lexem.h tokenizing.h
	$(CC) $(CFLAGS) -c $< -o $@
RPN.o: RPN.cpp RPN.h
	$(CC) $(CFLAGS) -c $< -o $@
parser.o: parser.cpp parser.h
	$(CC) $(CFLAGS) -c $< -o $@
table.o: table.cpp table.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(PROG)
run:
	./$(PROG)
valgrind:
	valgrind --tool=memcheck ./$(PROG)

CC=g++
CFLAGS=-c -Wall

all: main

main: main.o function.o solver.o
	$(CC) main.o function.o solver.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

function.o: function.cpp
	$(CC) $(CFLAGS) function.cpp

solver.o: solver.cpp
	$(CC) $(CFLAGS) solver.cpp

clean:
	del *.o *.json main.exe
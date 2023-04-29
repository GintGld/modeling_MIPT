CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp gas1D/gas1D.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=model

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

# for windows
clean:
	del /q /s *.o
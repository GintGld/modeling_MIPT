CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp RayTracing/ray.cpp RayTracing/space.cpp
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
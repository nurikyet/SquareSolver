CC=g++
CFLAGS=-c -Wall 
LDFLAGS=

SOURCES=Stack.cpp Main.cpp logfile.cpp Hash.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# TODO: What if header changes? Will your necessary file automatically recompile, or won't it?
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf hello *.o

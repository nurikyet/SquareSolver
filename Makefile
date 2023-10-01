CC=g++
CFLAGS=-c -Wall 
LDFLAGS=

SOURCES=Stack.cpp Main.cpp logfile.cpp Hash.cpp Error.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# TODO: What if header changes? Will your necessary file automatically recompile, or won't it?
#       DO NOT IGNORE ME
# Kirill, hint: .cpp.o might be rewritten like %.o : %.cpp
# Sasha's question is actual: do we need to add dependency of header file (%.h) to .o file generation? will version without %.h react to changes in corresponding header?
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf hello *.o

CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=Instructions.cpp labelMatcher.cpp SymbolHandler.cpp XEDissasemblerMain.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=xed

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
# Compiler
CC = g++

# Flags from pkg-config for libcurl
CURL_CFLAGS = $(shell pkg-config --cflags libcurl)
CURL_LDFLAGS = $(shell pkg-config --libs libcurl)

# Flags for ncurses library
NCURSES_LDFLAGS = -lncurses

# Compiler flags (combine curl cflags)
CFLAGS = $(CURL_CFLAGS)

# Linker flags (combine curl and ncurses libraries)
LDFLAGS = $(CURL_LDFLAGS) $(NCURSES_LDFLAGS)

# Source and header files
SRC = htmlread.cpp 
HEADERS = tree.h punctuate.h gui.h parser.h
OBJ = htmlread.o
TARGET = htmlparser

# Build target
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile source to object files
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

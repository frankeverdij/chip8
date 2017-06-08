# chip8 Makefile

# g++ compiler
CXX = g++

# g++ compiler flags
CXXFLAGS = -O2

# linker
LD = $(CXX)

# linker flags
LDFLAGS = -lSDL2

# name of executable
EXE = chip8

# list of all objects
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Makefile
all: $(OBJECTS)
	$(LD) -o $(EXE) $(OBJECTS) $(LDFLAGS)
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning everything
clean:
	rm -f $(EXE) $(OBJECTS)
# End

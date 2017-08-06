# chip8 Makefile

# g++ compiler
CXX = g++

# g++ compiler flags
CXXFLAGS = -O2 -std=c++11 -I.

# linker
LD = $(CXX)

# linker flags
LDFLAGS = -lSDL2 -lGL

# name of executable
EXE = chip8

# list of all sources
SOURCES = $(wildcard *.cpp)

# add SDL2 backend
SOURCES += $(wildcard sdl2/*.cpp)

# list of all sources
OBJECTS = $(SOURCES:.cpp=.o)

# Makefile
all: $(OBJECTS)
	$(LD) -o $(EXE) $(OBJECTS) $(LDFLAGS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning everything
clean:
	rm -f $(EXE) $(OBJECTS)
# End

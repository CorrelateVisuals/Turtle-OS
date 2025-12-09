CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src -I./src/shaders
LDFLAGS = -lGLESv2 -lglfw

# All .cpp → .o
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

BIN = bin/Turtle-OS

# Default rule
all: $(BIN)

# Link only recompiled objects
$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LDFLAGS)

# Compile each .cpp into .o only if changed
src/%.o: src/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean
clean:
	rm -f src/*.o $(BIN)

.PHONY: all clean

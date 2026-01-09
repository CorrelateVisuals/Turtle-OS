CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src -I./src/shaders
LDFLAGS = -lGLESv2 -lglfw

OBJDIR = obj
BINDIR = bin

# All .cpp → .o (in obj directory)
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=$(OBJDIR)/%.o)

BIN = $(BINDIR)/Turtle-OS

# Default rule
all: $(OBJDIR) $(BIN)

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $@

# Link only recompiled objects
$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LDFLAGS)

# Compile each .cpp into .o (in obj directory)
$(OBJDIR)/%.o: src/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean
clean:
	rm -rf $(OBJDIR) $(BIN)

.PHONY: all clean

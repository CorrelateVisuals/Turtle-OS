CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src -I./src/shaders
LDFLAGS = -lGLESv2 -lglfw

SRC = $(wildcard src/*.cpp)
BIN = bin/app

$(BIN): $(SRC)
	$(CXX) $(SRC) -o $(BIN) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(BIN)

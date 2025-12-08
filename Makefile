CC = gcc
CFLAGS = -I./src -I./src/shaders -Wall
LIBS = -lGLESv2 -lglfw

SRC = src/main.c src/window.c src/shader.c
BIN = bin/app

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(BIN) $(LIBS)

clean:
	rm -f $(BIN)

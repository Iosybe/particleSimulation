cc = g++
CFLAGS = -Wall -Wextra

all:
	$(cc) $(CFLAGS) -o build/main.out src/main.c src/shapes.c src/loadShader.cpp -lGL -lglfw -lGLEW

run: all
	./build/main.out

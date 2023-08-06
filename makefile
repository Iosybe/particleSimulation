cc = g++
CFLAGS = -Wall -Wextra

all:
	$(cc) $(CFLAGS) -o build/main.out src/main.c src/physics.c src/shapes.c src/globalStructs.c src/globalFunctions.c src/loadShader.cpp -lGL -lglfw -lGLEW

run: all
	./build/main.out

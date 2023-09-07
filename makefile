cc = g++
CFLAGS = -Wall -Wextra

all:
	$(cc) $(CFLAGS) -o build/main.out src/main.c src/physics.c src/draw.c src/helperFiles/globalStructs.c src/helperFiles/globalFunctions.c src/shaders/loadShader.cpp -lGL -lglfw -lGLEW

run: all
	./build/main.out

cc = g++
CFLAGS = -Wall -Wextra

all:
	$(cc) $(CFLAGS) -o main.out main.c loadShader.cpp -lGL -lglfw -lGLEW

run: all
	./main.out

#ifndef SHAPES_H
#define SHAPES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define UNUSED(x) (void)(x)
#define M_PI (3.14159265358979323846264338327950288)

void buildUnitCircle(GLfloat** circle, int segments);
void drawCircle(GLfloat* templateCircle, int segments, float centerX, float centerY, float radius);
void window_size_callback(GLFWwindow* window, int _width, int _height);

#endif
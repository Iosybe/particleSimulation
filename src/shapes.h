#ifndef SHAPES_H
#define SHAPES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define UNUSED(x) (void)(x)

void buildUnitCircle(GLfloat* circle, int segments);
void drawCircle(GLfloat* templateCircle, int segments, float centerX, float centerY, float radius);
void window_size_callback(GLFWwindow* window, int _width, int _height);

#endif
#ifndef DRAW_H
#define DRAW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define M_PI (3.14159265358979323846264338327950288)

void correctDrawing(GLuint program, int trackedParticle);

void buildUnitCircle(GLfloat** circle, int segments);
// void drawCircle(GLuint program, GLfloat* templateCircle, int segments, float centerX, float centerY, float radius);

void createCircleBuffer(GLfloat* circle, int segments, GLuint* buffer);
void drawCircleBufferless(int segments, int particleAmount);

#endif
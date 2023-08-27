#undef __STRICT_ANSI__
#include "draw.h"
#include "helperFiles/globalStructs.h"

#include <stdlib.h>
#include <math.h>

// Faster but less clean
float correctionX;
float correctionY;

float getCorrectedPosX(float posX) {
    return (posX + viewportState.transX) * correctionX;
}

float getCorrectedPosY(float posY) {
    return (posY + viewportState.transY) * correctionY;
}

float getCorrectedWidth(float width) {
    return width * correctionX;
}

float getCorrectedHeight(float height) {
    return height * correctionY;
}

void buildUnitCircle(GLfloat** circlePointer, int segments) {
    *circlePointer = (GLfloat*) malloc(sizeof(GLfloat) * (segments * 6));
    GLfloat* circle = *circlePointer;

    float angleInterval = M_PI * 2 / segments;

    for (int i = 0; i < segments; i++) {
        float currentAngle = angleInterval * i;

        int j = i * 6;

        circle[j + 0] = 0.0f;
        circle[j + 1] = 0.0f;

        circle[j + 2] = cos(currentAngle - angleInterval);
        circle[j + 3] = sin(currentAngle - angleInterval);

        // kan sneller. remember prev vertex.
        circle[j + 4] = cos(currentAngle);
        circle[j + 5] = sin(currentAngle);
    }
}

// Too avoid NOP divisions
void correctDrawing(GLuint program) {
    correctionX = viewportState.zoomScale / windowState.width;
    correctionY = viewportState.zoomScale / windowState.height;

    GLuint correctionLocation = glGetUniformLocation(program, "correction");
    glUniform2f(correctionLocation, correctionX, correctionY);

    GLuint transCorrectionLocation = glGetUniformLocation(program, "transCorrection");
    glUniform2f(transCorrectionLocation, viewportState.transX, viewportState.transY);
}

// void drawCircle(GLuint program, GLfloat* circle, int segments, float posX, float posY, float radius) {
//     GLuint vertexbuffer;
//     createCircleBuffer(circle, segments, &vertexbuffer);

//     drawCircleBufferless(program, circle, segments, posX, posY, radius);

//     glDisableVertexAttribArray(0);
//     glDeleteBuffers(1, &vertexbuffer);
// }

void createCircleBuffer(GLfloat* circle, int segments, GLuint* buffer) {
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * segments * 6, circle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void drawCircleBufferless(int segments, int particleAmount) {
    glDrawArraysInstanced(GL_TRIANGLES, 0, segments * 3, particleAmount);
}
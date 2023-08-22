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
void correctDrawing() {
    correctionX = viewportState.zoomScale / windowState.width;
    correctionY = viewportState.zoomScale / windowState.height;
}

void drawCircle(GLuint program, GLfloat* circle, int segments, float posX, float posY, float radius) {
    GLuint vertexbuffer;
    createCircleBuffer(circle, segments, &vertexbuffer);

    drawCircleBufferless(program, circle, segments, posX, posY, radius);

    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexbuffer);
}

void createCircleBuffer(GLfloat* circle, int segments, GLuint* buffer) {
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * segments * 6, circle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        0                   // offset
    );

    // glDisableVertexAttribArray(0);
    // glDeleteBuffers(1, &vertexbuffer);
}

void drawCircleBufferless(GLuint program, GLfloat* circle, int segments, float posX, float posY, float radius) {
    // only getting locations once would be faster
    GLuint translationLocation = glGetUniformLocation(program, "translation");
    glUniform2f(translationLocation, getCorrectedPosX(posX), getCorrectedPosY(posY));

    GLuint scaleLocation = glGetUniformLocation(program, "scale");
    glUniform2f(scaleLocation, getCorrectedWidth(radius), getCorrectedHeight(radius));

    glDrawArrays(GL_TRIANGLES, 0, segments * 3);
}
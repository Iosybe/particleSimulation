#undef __STRICT_ANSI__
#include "draw.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#include <stdlib.h>

void window_size_callback(GLFWwindow* window, int _width, int _height) {
    windowState.width = _width;
    windowState.height = _height;

    UNUSED(window);
}

void buildUnitCircle(GLfloat** circlePointer, int segments) {
    *circlePointer = (GLfloat*) malloc(sizeof(GLfloat) * (segments * 18));
    GLfloat* circle = *circlePointer;

    float angleInterval = M_PI * 2 / segments;

    for (int i = 0; i < segments; i++) {
        float currentAngle = angleInterval * i;

        int j = i * 9;

        circle[j] = 0.0f;
        circle[j + 1] = 0.0f;
        circle[j + 2] = 0.0f;

        circle[j + 3] = cos(currentAngle - angleInterval);
        circle[j + 4] = sin(currentAngle - angleInterval);
        circle[j + 5] = 0.0f;

        // kan sneller. remember prev vertex.
        circle[j + 6] = cos(currentAngle);
        circle[j + 7] = sin(currentAngle);
        circle[j + 8] = 0.0f;
    }
}

void drawCircle(GLfloat* templateCircle, int segments, float posX, float posY, float radius) {
    GLfloat* circle = templateCircle + segments * 9;

    float correctedPosX = (posX + viewportState.transX) * viewportState.zoomScale / windowState.width;
    float correctedPosY = (posY + viewportState.transY) * viewportState.zoomScale / windowState.height;
    float correctedRadiusX = radius * viewportState.zoomScale / windowState.width;
    float correctedRadiusY = radius * viewportState.zoomScale / windowState.height;

    for (int i = 0; i < segments * 9; i += 9) {

        circle[i + 0] = correctedRadiusX * templateCircle[i + 0] + correctedPosX;
        circle[i + 1] = correctedRadiusY * templateCircle[i + 1] + correctedPosY;
        circle[i + 2] = 0.0f;

        circle[i + 3] = correctedRadiusX * templateCircle[i + 3] + correctedPosX;
        circle[i + 4] = correctedRadiusY * templateCircle[i + 4] + correctedPosY;
        circle[i + 5] = 0.0f;

        circle[i + 6] = correctedRadiusX * templateCircle[i + 6] + correctedPosX;
        circle[i + 7] = correctedRadiusY * templateCircle[i + 7] + correctedPosY;
        circle[i + 8] = 0.0f;
    }

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * segments * 9, circle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, segments * 3); // Starting from vertex 0; 3 vertices total . 1 triangle
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexbuffer);
}
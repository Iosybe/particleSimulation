#undef __STRICT_ANSI__
#include "shapes.h"
#include "globalStructs.h"
#include <math.h>


int width = 500;
int height = 500;

void window_size_callback(GLFWwindow* window, int _width, int _height) {
    width = _width;
    height = _height;

    UNUSED(window);
}

void buildUnitCircle(GLfloat* circle, int segments) {
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
    GLfloat circle[segments * 9];

    float correctedPosX = posX * viewportState.zoomScale + viewportState.transX;
    float correctedPosY = posY * viewportState.zoomScale + viewportState.transY;
    float correctedRadius = radius * viewportState.zoomScale;

    for (int i = 0; i < segments * 9; i += 9) {

        circle[i + 0] = (correctedRadius * templateCircle[i + 0] + correctedPosX) / width;
        circle[i + 1] = (correctedRadius * templateCircle[i + 1] + correctedPosY) / height;
        circle[i + 2] = 0.0f;

        circle[i + 3] = (correctedRadius * templateCircle[i + 3] + correctedPosX) / width;
        circle[i + 4] = (correctedRadius * templateCircle[i + 4] + correctedPosY) / height;
        circle[i + 5] = 0.0f;

        circle[i + 6] = (correctedRadius * templateCircle[i + 6] + correctedPosX) / width;
        circle[i + 7] = (correctedRadius * templateCircle[i + 7] + correctedPosY) / height;
        circle[i + 8] = 0.0f;
    }

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

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
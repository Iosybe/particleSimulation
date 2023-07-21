#include "shapes.h"
#include <math.h>

int width = 500;
int height = 500;

void window_size_callback(GLFWwindow* window, int _width, int _height) {
    width = _width;
    height = _height;

    UNUSED(window);
}

void aspect_ratio_correction(GLfloat* circle, int segments) {
    for (int i = 0; i < segments * 9; i += 3) {
        circle[i] = circle[i] / width;
        circle[i + 1] = circle[i + 1] / height; 
    }
}

void buildCircle(GLfloat* circle, float radius, int segments) {
    float angleInterval = M_PI * 2 / segments;

    for (int i = 0; i < segments; i++) {
        float currentAngle = angleInterval * i;

        int j = i * 9;

        circle[j] = 0.0f;
        circle[j + 1] = 0.0f;
        circle[j + 2] = 0.0f;

        circle[j + 3] = radius * cos(currentAngle - angleInterval);
        circle[j + 4] = radius * sin(currentAngle - angleInterval);
        circle[j + 5] = 0.0f;

        // kan sneller. remember prev vertex.
        circle[j + 6] = radius * cos(currentAngle);
        circle[j + 7] = radius * sin(currentAngle);
        circle[j + 8] = 0.0f;
    }
}

void drawCircle(GLfloat* templateCircle, int segments, float centerX, float centerY) {
    GLfloat circle[segments * 9];

    for (int i = 0; i < segments * 9; i += 9) {

        circle[i + 0] = templateCircle[i + 0] + centerX;
        circle[i + 1] = templateCircle[i + 1] + centerY;
        circle[i + 2] = 0.0f;

        circle[i + 3] = templateCircle[i + 3] + centerX;
        circle[i + 4] = templateCircle[i + 4] + centerY;
        circle[i + 5] = 0.0f;

        circle[i + 6] = templateCircle[i + 6] + centerX;
        circle[i + 7] = templateCircle[i + 7] + centerY;
        circle[i + 8] = 0.0f;
    }

    aspect_ratio_correction(circle, segments);

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
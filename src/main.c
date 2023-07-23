#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/resource.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "loadShader.hpp"
#include "shapes.h"
#include "physics.h"

#define NOP 5000
#define SEGMENTS 128

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    UNUSED(window);
}

int main() {
    const rlim_t kStackSize = 256 * 1024 * 1024;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

    GLFWwindow* window;
    window = glfwCreateWindow( 500, 500, "ParticleSim", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    if ( window == NULL ) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = 1; // Needed in core profile

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLfloat circle[SEGMENTS * 9];
    buildCircle(circle, 4.0, SEGMENTS);

    srand(time(NULL));

    Particle particles[NOP];
    initializeParticles(particles, NOP);
    int bufSize = NOP / 2.0 * (NOP - 1);

    GLuint programID = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );

    // time_t prevTime = time(NULL);
    // int fps = 0;

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        // for (int i = 0; i < 100; i++) {
        //     drawCircle(circle, 128, (float) (rand() % 1000) - 500, (float) (rand() % 1000) - 500);
        // }

        calculatePhysics(particles, NOP, bufSize);

        // return 1;

        for (int i = 0; i < NOP; i++) {
            drawCircle(circle, SEGMENTS, particles[i].posX, particles[i].posY);
        }

        // while (prevTime + 100 > clock()) {
        //     glfwPollEvents();
        // }
 
        // if (prevTime < time(NULL)) {
        //     printf("%i\n", fps);

        //     fps = 0;
        //     prevTime++;
        // }
        // else {
        //     fps++;
        // }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while( glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glfwDestroyWindow(window);

    return 0;
}

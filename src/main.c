#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/resource.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaders/loadShader.hpp"
#include "glfw/glfwCallbacks.h"
#include "shapes.h"
#include "physics.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#define SEGMENTS 128
GLfloat circle[SEGMENTS * 9];
Particle particles[NOP];

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    UNUSED(window);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &viewportState.prevPosX, &viewportState.prevPosY);
    }
    UNUSED(mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0.01);
        viewportState.zoomScale *= (float)yoffset;
        printf("%f\n", yoffset);
    
    
    UNUSED(window);
    UNUSED(xoffset);
}

static void cursor_position_callback(GLFWwindow* window, double cursorPosX, double cursorPosY) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        return;
    }
    viewportState.trackedParticle = -1;

    viewportState.transX += 2 * (cursorPosX - viewportState.prevPosX) / viewportState.zoomScale;
    viewportState.transY += 2 * (viewportState.prevPosY - cursorPosY) / viewportState.zoomScale;
    
    viewportState.prevPosX = cursorPosX;
    viewportState.prevPosY = cursorPosY;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* primaryMonitorMode = glfwGetVideoMode(primaryMonitor);
        
        if (windowState.fullscreenState) {
            glfwSetWindowMonitor(window, NULL, 0, 0, primaryMonitorMode->width, primaryMonitorMode->height, primaryMonitorMode->refreshRate);
            windowState.fullscreenState = !windowState.fullscreenState;
        }
        else {
            glfwSetWindowMonitor(window, primaryMonitor, 0, 0, primaryMonitorMode->width, primaryMonitorMode->height, primaryMonitorMode->refreshRate);
            windowState.fullscreenState = !windowState.fullscreenState;
        }
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        viewportState.trackedParticle = (int) randFloat((float) NOP);
    }

    if (key == GLFW_KEY_MINUS && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        viewportState.zoomScale *= 0.9;
    }

    if (key == GLFW_KEY_EQUAL && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        viewportState.zoomScale *= 1.1;
    }

    UNUSED(mods);
    UNUSED(scancode);
}

int main(void) {
    // const rlim_t kStackSize = 256 * 1024 * 1024;
    // struct rlimit rl;
    // int result;

    // result = getrlimit(RLIMIT_STACK, &rl);
    // if (result == 0)
    // {
    //     if (rl.rlim_cur < kStackSize)
    //     {
    //         rl.rlim_cur = kStackSize;
    //         result = setrlimit(RLIMIT_STACK, &rl);
    //         if (result != 0)
    //         {
    //             fprintf(stderr, "setrlimit returned result = %d\n", result);
    //         }
    //     }
    // }

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
    // glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    // glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

    GLFWwindow* window;
    window = glfwCreateWindow( windowState.width, windowState.height, "ParticleSim", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeLimits(window, 10, 10, GLFW_DONT_CARE, GLFW_DONT_CARE);

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

    // glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    
    buildUnitCircle(circle, SEGMENTS);

    srand(time(NULL));

    Particle particles[NOP];

    if (initializeParticles(particles)) {
        return 1;
    }

    GLuint programID = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );

    // time_t prevTime = time(NULL);
    // int fps = 0;

    

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        calculatePhysics(particles);
        
        if (viewportState.trackedParticle != -1) {
            viewportState.transX = -particles[viewportState.trackedParticle].posX;
            viewportState.transY = -particles[viewportState.trackedParticle].posY;
        }

        for (int i = 0; i < NOP; i++) {
            drawCircle(circle, SEGMENTS, particles[i].posX, particles[i].posY, particles[i].mass);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while( glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glfwDestroyWindow(window);

    destroyParticles(particles);

    return 0;
}

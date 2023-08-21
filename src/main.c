#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/loadShader.h"
#include "glfw/glfwCallbacks.h"
#include "draw.h"
#include "physics.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#define NOP 1000 // Number of particle
#define SEGMENTS 128 // Segment in a circle

int main(void) {
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

    GLFWwindow* window;
    window = glfwCreateWindow( windowState.width, windowState.height, "ParticleSim", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeLimits(window, 10, 10, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetScrollCallback(window, scroll_callback);

    if ( window == NULL ) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Initialize GLEW
    // glewExperimental = 1; // Needed in core profile

    // if (glewInit() != GLEW_OK) {
    //     fprintf(stderr, "Failed to initialize GLEW\n");
    //     return -1;
    // }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD");
		return -1;
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLfloat* circle;
    buildUnitCircle(&circle, SEGMENTS);

    if (initializeParticles(NOP)) { return 1; }
    initializeGlfwCallbacks(NOP);

    Particle* particles = getParticles();

    GLuint programID = LoadShaders( "shaders/SimpleVertexShader.vert", "shaders/SimpleFragmentShader.frag" );

    double prevTime = glfwGetTime();

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        glfwPollEvents();

        if (simulationState.pauze == 0) {
            updatePhysics();
        }
        
        if (viewportState.trackedParticle != -1) {
            viewportState.transX = -particles[viewportState.trackedParticle].posX;
            viewportState.transY = -particles[viewportState.trackedParticle].posY;
        }

        for (int i = 0; i < NOP; i++) {
            drawCircle(circle, SEGMENTS, particles[i].posX, particles[i].posY, particles[i].mass);
        }

        glfwSwapBuffers(window);

        double curTime = glfwGetTime();
        printf("fps: %i\n", (int) (1.0 / (curTime - prevTime)));
        prevTime = curTime;
    }
    while( glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glfwDestroyWindow(window);

    destroyParticles(particles);

    return 0;
}

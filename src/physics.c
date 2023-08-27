#include "physics.h"
#include "shaders/loadShader.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int nop; // Number of particles
GLuint physicsProgramID;
GLuint sumVelProgramID;

GLuint particlesBuffer;
GLuint velocityBuffer;

// Particle* getParticles() {
//     return particles;
// }

int initializeParticles(int _nop) {
    nop = _nop;

    // Creating Particles
    Particle* particles = (Particle*) malloc(sizeof(Particle) * nop);
    if (particles == NULL) { return 1; }

    for (int i = 0; i < nop; i++) {
        float x = randFloatRange(-200.0, 200.0);
        float y = randFloatRange(-200.0, 200.0);

        particles[i] = (Particle) {
            .pos = {x, y},
            .vel = {-y / 100.0, x / 100.0},
            .mass = randFloat(5.0),
        };
    }

    glGenBuffers(1, &particlesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlesBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * nop, particles, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particlesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    free(particles);

    // printf("%i\n", 2 * sizeof(float) * (nop * nop - nop) + sizeof(int));

    // Creating velocity buffer
    glGenBuffers(1, &velocityBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 2 * sizeof(float) * (nop * nop - nop) + sizeof(int), NULL, GL_DYNAMIC_DRAW); // Should be changed to fixed size
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocityBuffer);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(int), &nop);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    physicsProgramID = LoadComputeShaders("shaders/physics.comp");
    sumVelProgramID = LoadComputeShaders("shaders/sumVel.comp");

    return 0;
}

void destroyParticles() {
    glDeleteProgram(physicsProgramID);

    glDeleteBuffers(1, &particlesBuffer);
    glDeleteBuffers(1, &velocityBuffer);
}

void updatePhysics() {
    glUseProgram(physicsProgramID);
    glDispatchCompute(nop / 8, nop / 4, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(sumVelProgramID);
    glDispatchCompute(nop / 32, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(0);
}
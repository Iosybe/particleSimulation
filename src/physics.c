#include "physics.h"
#include "shaders/loadShader.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>

int nop; // Number of particles
GLuint physicsProgramID;
GLuint sumVelProgramID;

GLuint particlesBuffer;
GLuint velocityBuffer;

int initializeParticles(int _nop) {
    nop = _nop;

    // Creating Particles
    int* particlesWnop = (int*) malloc(sizeof(Particle) * nop + sizeof(int) * 2);
    if (particlesWnop == NULL) { return 1; }

    *particlesWnop = nop - 1;
    Particle* particles = (Particle*) (particlesWnop + 2);

    for (int i = 0; i < nop; i++) {
        float x = randFloatRange(-200.0, 200.0);
        float y = randFloatRange(-200.0, 200.0);

        particles[i] = (Particle) {
            .pos = {x, y},
            .vel = {-y / 100.0, x / 100.0},
            .mass = randFloat(5.0),
        };

        // particles[i] = (Particle) {
        //     .pos = {0.0, 0.2},
        //     .vel = {0.4, 0.6},
        //     .mass = 0.8,
        // };
    }

    glGenBuffers(1, &particlesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlesBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * nop + sizeof(int) * 2, particlesWnop, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particlesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    free(particlesWnop);

    // Creating velocity buffer
    glGenBuffers(1, &velocityBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 2 * sizeof(float) * (nop * nop - nop), NULL, GL_DYNAMIC_DRAW); // Should be changed to fixed size
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velocityBuffer);
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
    glDispatchCompute(nop / 16, nop / 8, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(sumVelProgramID);
    glDispatchCompute(nop / 128, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(0);
}
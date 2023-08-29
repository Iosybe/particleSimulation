#include "physics.h"
#include "shaders/loadShader.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>

int nop; // Number of particles

int workGroupsPhysics[2];
int workGroupsSumVel;

GLuint physicsProgramID;
GLuint sumVelProgramID;

GLuint particlesBuffer;
GLuint velocityBuffer;

int initializeParticles(int _nop) {
    nop = _nop;

    int nopX16 = nop + 16 * (nop % 16 > 0); // nop rounded up to 16
    int nopX128 = nop + 128 * (nop & 128 > 0); // nop rounded up to 128

    workGroupsPhysics[0] = nop / 16 + (nop % 16 > 0);
    workGroupsPhysics[1] = nop / 8 + (nop % 8 > 0);

    workGroupsSumVel = nop / 128 + (nop % 128 > 0);

    // workGroupsPhysics[0] = nop / 16 + ;
    // workGroupsPhysics[1] = nop / 2 / 8 + (nop % 8 > 0);

    // workGroupsSumVel = nop / 128 + (nop % 128 > 0);

    // Creating Particles
    int* particlesWnop = (int*) malloc(sizeof(Particle) * nopX16 + sizeof(int) * 2);
    if (particlesWnop == NULL) { return 1; }

    *particlesWnop = nop - 1;
    *(particlesWnop + 1) = nopX128 - 1;
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
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * nopX16 + sizeof(int) * 2, particlesWnop, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particlesBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    free(particlesWnop);

    // Creating velocity buffer
    glGenBuffers(1, &velocityBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocityBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 2 * sizeof(float) * (nopX128 * nopX128 - nopX128), NULL, GL_DYNAMIC_DRAW); // Should be changed to fixed size
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
    glDispatchCompute(workGroupsPhysics[0], workGroupsPhysics[1], 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(sumVelProgramID);
    glDispatchCompute(workGroupsSumVel, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(0);
}
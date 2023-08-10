#ifndef PHYSICS_H
#define PHYSICS_H

#include <pthread.h>

#define NOP 1000 // Number of particle
#define NOIPT 60000 // Number of interactions per thread

typedef struct particle {
    int id;
    float mass;

    float posX;
    float posY;

    float velX;
    float velY;

    pthread_mutex_t mutex;
} Particle;

typedef struct combCouple {
    Particle* particleOne;
    Particle* particleTwo;
} CombCouple;

typedef struct InteractionData {
    CombCouple* startPointer;
    CombCouple* endPointer;
} InteractionData;

int initializeParticles(Particle* particles);
void destroyParticles(Particle* particles);

void calculatePhysics(Particle* particles);

#endif
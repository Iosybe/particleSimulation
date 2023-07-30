#ifndef PHYSICS_H
#define PHYSICS_H

#include <pthread.h>

#define NOP 5000 // Number of particle

#define NOI (int) (NOP / 2.0 * (NOP - 1)) // Number of interactions
#define NOIPT 60000 // Number of interactions per thread
#define NOT (int) NOI / NOIPT + (NOI % NOIPT > 0) // Number of threads

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
void destroyParicles(Particle* particles);

void calculatePhysics(Particle* particles);

#endif
#ifndef PHYSICS_H
#define PHYSICS_H

#include <tinycthread.h>

#define NOIPT 60000 // Number of interactions per thread

typedef struct particle {
    int id;
    float mass;

    float posX;
    float posY;

    float velX;
    float velY;

    // pthread_mutex_t mutex; 
    mtx_t mutex;
} Particle;

typedef struct combCouple {
    Particle* particleOne;
    Particle* particleTwo;
} CombCouple;

typedef struct InteractionData {
    CombCouple* startPointer;
    CombCouple* endPointer;
} InteractionData;

Particle* getParticles();

int initializeParticles(int _nop);
void destroyParticles();
void updatePhysics();

#endif
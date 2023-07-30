#ifndef PHYSICS_H
#define PHYSICS_H

#include <pthread.h>

#define NOP 5000
#define BUFSIZE (int) (NOP / 2.0 * (NOP - 1))

typedef struct particle {
    int id;
    float mass;

    float posX;
    float posY;

    float velX;
    float velY;

    pthread_mutex_t mutex;
} Particle;

typedef struct InteractionData {
    int i;
    Particle* particles;
} InteractionData;

void initializeParticles(Particle* particles);
void calculatePhysics(Particle* particles);

#endif
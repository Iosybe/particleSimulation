#ifndef PHYSICS_H
#define PHYSICS_H

#include <tinycthread.h>

typedef struct particle {
    float pos[2];
    float vel[2];
    float mass;
    // padding because vec are gay
    float padding;
} Particle;

int initializeParticles(int _nop);
void destroyParticles();
void updatePhysics();

#endif
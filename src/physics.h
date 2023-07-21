#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct particle {
    int id;
    float mass;

    float posX;
    float posY;

    float velX;
    float velY;
} Particle;

void initializeParticles(Particle* particles, int particleAmount);
void calculatePhysics(Particle* particles, int particleAmount);

#endif
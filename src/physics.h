#ifndef PHYSICS_H
#define PHYSICS_H

#define NOP 1001
#define BUFSIZE (int) (NOP / 2.0 * (NOP - 1))

typedef struct particle {
    int id;
    float mass;

    float posX;
    float posY;

    float velX;
    float velY;
} Particle;

void initializeParticles(Particle* particles);
void calculatePhysics(Particle* particles);

#endif
#include "physics.h"

#include <stdlib.h>

// Generates random float that can't be zero
float randFloat(float max) {
    return ((float) rand()) + 1 / (float) RAND_MAX * max;
}

float randFloatRange(float min, float max) {
    return ((float) rand()) / (float) RAND_MAX * (max * 2) + min;
}

void initializeParticles(Particle* particles, int particleAmount) {
    for (int i = 0; i < particleAmount; i++) {
        particles[i] = {
            i,
            randFloat(5.0),

            randFloatRange(-500.0, 500.0),
            randFloatRange(-500.0, 500.0),

            0.0,
            0.0,
        };
    }
}

float calcDistance(float diffX, float diffY) {
    return sqrt(diffX * diffX + diffY * diffY);
}

void calculatePhysics(Particle* particles, int particleAmount) {
    for (int i = 0; i < particleAmount; i++) {
        for (int j = 0; j < particleAmount; j++) {
            float diffX = particle[i]->posX - particle[j]->posX;
            float diffY = particle[i]->posY - particle[j]->posY;
            float gravFactor = particles[i].mass * particles[j].mass / pow(calcDistance(diffX, diffY), 3);
            particles[i].velX += gravFactor * diffX;
            particles[i].velY += gravFactor * diffY;

            particles[i].posX += particles[i].velX;
            particles[i].posY += particles[i].velY;
        }
    }


}
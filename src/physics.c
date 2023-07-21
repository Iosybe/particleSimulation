#include "physics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Generates random float that can't be zero
float randFloat(float max) {
    return (((float) rand()) + 1) / (float) RAND_MAX * max;
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
    // printf("%f - %f\n", particles[1].posX, particles[1].posY);

    for (int i = 0; i < particleAmount; i++) {
        // printf("%f - %f\n", particles[i].posX, particles[i].posY);

        for (int j = 0; j < particleAmount; j++) {
            if (i == j) {
                continue;
            }
            float diffX = particles[i].posX - particles[j].posX;
            float diffY = particles[i].posY - particles[j].posY;
            float distance = calcDistance(diffX, diffY);
            if (distance > 50) {
                float gravFactor = - particles[j].mass / pow(distance, 3);
                particles[i].velX += gravFactor * diffX;
                particles[i].velY += gravFactor * diffY;

                particles[i].posX += particles[i].velX;
                particles[i].posY += particles[i].velY;
            } 
            
        }
    }

}
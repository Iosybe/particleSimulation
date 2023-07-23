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

// Optimization because pow() is slow
float powTwo(float n) {
    return n * n;
}

float powThree(float n) {
    return n * n * n;
}

float calcDistance(float diffX, float diffY) {
    return sqrt(powTwo(diffX) + powTwo(diffY));
}

void calculatePhysics(Particle* particles, int particleAmount) {
    // Creating buffer arrays
    int bufSize = particleAmount / 2.0 * (particleAmount - 1);
    float gravFactorBuf[bufSize];
    float diffXBuf[bufSize];
    float diffYBuf[bufSize];

    int bufIndex = 0;

    for (int i = 0; i < particleAmount; i++) {
        for (int j = i + 1; j < particleAmount; j++) {
            float diffX = particles[i].posX - particles[j].posX;
            float diffY = particles[i].posY - particles[j].posY;

            float distance = calcDistance(diffX, diffY);

            // Maybe (distance > 50) * gravFactor would be faster 
            gravFactorBuf[bufIndex] = distance > 50 ? -particles[j].mass / powThree(distance) : 0;

            diffXBuf[bufIndex] = diffX;
            diffYBuf[bufIndex] = diffY;

            bufIndex++;
        }
    }

    // printf("test\n");
    // Selecting from buffer array
    int topStartIndex = 0;

    for (int i = 0; i < particleAmount; i++) {
        // Top half
        if (i < particleAmount - 1) {
            for (int j = topStartIndex;; j++) {
                if (j >= topStartIndex + particleAmount - i - 2) {
                    topStartIndex = j + 1;
                    break;
                }

                particles[i].velX += gravFactorBuf[j] * diffXBuf[j];
                particles[i].velY += gravFactorBuf[j] * diffYBuf[j];
            }
        }

        // Bottom half
        if (i > 0) {
            int k = particleAmount - 1;
            int l = i - 1;

            for (int j = 0; j < i; j++) {
                particles[i].velX += gravFactorBuf[l] * -diffXBuf[l];
                particles[i].velY += gravFactorBuf[l] * -diffYBuf[l];

                // maybe l += k--
                k--;
                l += k;
            }
        }

        particles[i].posX += particles[i].velX;
        particles[i].posY += particles[i].velY;
    }

    // printf("%f - %f\n", particles[5].velX, particles[5].velY);
}

// void calculatePhysics(Particle* particles, int particleAmount) {
//     // printf("%f - %f\n", particles[1].posX, particles[1].posY);

//     for (int i = 0; i < particleAmount; i++) {
//         // printf("%f - %f\n", particles[i].posX, particles[i].posY);

//         for (int j = 0; j < particleAmount; j++) {
//             if (i == j) { continue; }

//             float diffX = particles[i].posX - particles[j].posX;
//             float diffY = particles[i].posY - particles[j].posY;

//             float distance = calcDistance(diffX, diffY);

//             if (distance > 50) {
//                 float gravFactor = -particles[j].mass / powThree(distance);

//                 particles[i].velX += gravFactor * diffX;
//                 particles[i].velY += gravFactor * diffY;
//             } 
//         }

//         particles[i].posX += particles[i].velX;
//         particles[i].posY += particles[i].velY;
//     }
//         // printf("%f - %f\n", particles[5].velX, particles[5].velY);

// }
#include "physics.h"
#include "globalStructs.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

// Generates random float that is between 1 and max + 1
float randFloat(float max) {
    return 1 + (float) rand() * max / (float) RAND_MAX;
}

float randFloatRange(float min, float max) {
    return min + (float) rand() * (max - min) / (float) RAND_MAX;
}

void initializeParticles(Particle* particles) {
    for (int i = 0; i < NOP; i++) {
        float x = randFloatRange(-200.0, 200.0);
        float y = randFloatRange(-200.0, 200.0);
        particles[i] = (Particle) {
            i,
            randFloat(5.0),

            x,
            y,    

            -y / (float) 100.0,
            x / (float) 100.0,
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

float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

void calculatePhysics(Particle* particles) {
    struct timeval t0;
    struct timeval t1;
    gettimeofday(&t0, 0);

    // Creating buffer arrays
    // static float gravFactorBuf[BUFSIZE];
    // static float diffXBuf[BUFSIZE];
    // static float diffYBuf[BUFSIZE];

    for (int i = 0; i < NOP; i++) {
        for (int j = i + 1; j < NOP; j++) {
            float diffX = particles[i].posX - particles[j].posX;
            float diffY = particles[i].posY - particles[j].posY;

            float distance = calcDistance(diffX, diffY);

            if (distance > 20) {
                float distanceWeight = 1.0 / powThree(distance); 

                particles[i].velX += -gravConst * particles[j].mass * diffX * distanceWeight;
                particles[i].velY += -gravConst * particles[j].mass * diffY * distanceWeight;

                particles[j].velX -= -gravConst * particles[i].mass * diffX * distanceWeight;
                particles[j].velY -= -gravConst * particles[i].mass * diffY * distanceWeight;
            }
        }

        particles[i].posX += particles[i].velX;
        particles[i].posY += particles[i].velY;
    }

    gettimeofday(&t1, 0);
    printf("%f\n", timedifference_msec(t0, t1));
}
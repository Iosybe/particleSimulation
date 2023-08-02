#include "physics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

// Generates random float that can't be zero
float randFloat(float max) {
    return (((float) rand()) + 1) / (float) RAND_MAX * max;
}

float randFloatRange(float min, float max) {
    return ((float) rand()) / (float) RAND_MAX * (max * 2) + min;
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

            -y / 100.0,
            x / 100.0,
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

                particles[i].velX += -particles[j].mass * distanceWeight * diffX;
                particles[i].velY += -particles[j].mass * distanceWeight * diffY;

                particles[j].velX -= -particles[i].mass * distanceWeight * diffX;
                particles[j].velY -= -particles[i].mass * distanceWeight * diffY;
            }
        }

        particles[i].posX += particles[i].velX;
        particles[i].posY += particles[i].velY;
    }

    gettimeofday(&t1, 0);
    printf("%f\n", timedifference_msec(t0, t1));
}
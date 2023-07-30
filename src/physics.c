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
        particles[i] = (Particle) {
            i,
            randFloat(5.0),

            randFloatRange(-500.0, 500.0),
            randFloatRange(-500.0, 500.0),

            0.0,
            0.0,
        };

        pthread_mutex_init(&particles[i].mutex, NULL);
    }
}

// destroy particles function

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

void* particleInteractions(void* args) {
    Particle* particles = ((InteractionData*) args)->particles;
    int i = ((InteractionData*) args)->i;

    for (int j = i + 1; j < NOP; j++) {
        float diffX = particles[i].posX - particles[j].posX;
        float diffY = particles[i].posY - particles[j].posY;

        float distance = calcDistance(diffX, diffY);

        if (distance > 50) {
            float tempGravFactor = 1 / powThree(distance);

            float gravFactorI= -particles[j].mass * tempGravFactor;
            float gravFactorJ= -particles[i].mass * tempGravFactor;

            pthread_mutex_lock(&particles[i].mutex);
            particles[i].velX += gravFactorI * diffX;
            particles[i].velY += gravFactorI * diffY;
            pthread_mutex_unlock(&particles[i].mutex);

            pthread_mutex_lock(&particles[j].mutex);
            particles[j].velX -= gravFactorJ * diffX;
            particles[j].velY -= gravFactorJ * diffY;
            pthread_mutex_unlock(&particles[j].mutex);
        }
    }

    return NULL;
}

void calculatePhysics(Particle* particles) {
    struct timeval t0;
    struct timeval t1;
    gettimeofday(&t0, 0);

    pthread_t threadIds[NOP];
    InteractionData interactionDatas[NOP];

    for (int i = 0; i < NOP; i++) {
        interactionDatas[i] = (InteractionData) {
            i,
            particles,
        };

        pthread_create(&threadIds[i], NULL, particleInteractions, (void*) (interactionDatas + i));
    }

    for (int i = 0; i < NOP; i++) {
        pthread_join(threadIds[i], NULL);

        particles[i].posX += particles[i].velX;
        particles[i].posY += particles[i].velY;
    }


    gettimeofday(&t1, 0);
    printf("%f\n", timedifference_msec(t0, t1));
}
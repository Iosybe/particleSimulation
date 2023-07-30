#include "physics.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

CombCouple* combArray;

// Generates random float that can't be zero
float randFloat(float max) {
    return (((float) rand()) + 1) / (float) RAND_MAX * max;
}

float randFloatRange(float min, float max) {
    return ((float) rand()) / (float) RAND_MAX * (max * 2) + min;
}

int initializeParticles(Particle* particles) {
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

    combArray = (CombCouple*) malloc(sizeof(CombCouple) * NOI);

    if (combArray == NULL) {
        return 1;
    }

    int combIndex = 0;

    for (int i = 0; i < NOP; i++) {
        for (int j = i + 1; j < NOP; j++) {
            combArray[combIndex] = (CombCouple) {
                particles + i,
                particles + j,
            };

            combIndex++;
        }
    }

    return 0;
}

void destroyParicles(Particle* particles) {
    for (int i = 0; i < NOP; i++) {
        pthread_mutex_destroy(&particles[i].mutex);
    }

    free(combArray);
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

void* particleInteractions(void* args) {
    InteractionData* interactionData = (InteractionData*) args;

    for (CombCouple* particles = interactionData->startPointer; particles <= interactionData->endPointer; particles++) {
        Particle* particleOne = particles->particleOne;
        Particle* particleTwo = particles->particleTwo;

        float diffX = particleOne->posX - particleTwo->posX;
        float diffY = particleOne->posY - particleTwo->posY;

        float distance = calcDistance(diffX, diffY);

        if (distance > 50) {
            float tempGravFactor = 1 / powThree(distance);

            float gravFactorI= -particleTwo->mass * tempGravFactor;
            float gravFactorJ= -particleOne->mass * tempGravFactor;

            pthread_mutex_lock(&particleOne->mutex);
            particleOne->velX += gravFactorI * diffX;
            particleOne->velY += gravFactorI * diffY;
            pthread_mutex_unlock(&particleOne->mutex);

            pthread_mutex_lock(&particleTwo->mutex);
            particleTwo->velX -= gravFactorJ * diffX;
            particleTwo->velY -= gravFactorJ * diffY;
            pthread_mutex_unlock(&particleTwo->mutex);
        }
    }

    return NULL;
}

void calculatePhysics(Particle* particles) {
    struct timeval t0;
    struct timeval t1;
    gettimeofday(&t0, 0);

    pthread_t threadIds[NOT];
    InteractionData interactionDatas[NOT];

    int i = 0;
    CombCouple* p = combArray;

    for (; p + NOIPT - 1 < combArray + NOI; p += NOIPT, i++) {
        interactionDatas[i] = (InteractionData) {
            p,
            p + NOIPT - 1,
        };

        pthread_create(&threadIds[i], NULL, particleInteractions, (void*) (interactionDatas + i));
    }

    if (p < combArray + NOI) {
        interactionDatas[i] = (InteractionData) {
            p,
            combArray + NOI - 1,
        };

        pthread_create(&threadIds[i], NULL, particleInteractions, (void*) (interactionDatas + i));

        i++;
    }

    for (int j = 0; j < i; j++) {
        pthread_join(threadIds[j], NULL);
    }

    for (int j = 0; j < NOP; j++) {
        particles[j].posX += particles[j].velX;
        particles[j].posY += particles[j].velY;
    }

    gettimeofday(&t1, 0);
    printf("%f\n", timedifference_msec(t0, t1));
}
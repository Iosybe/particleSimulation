#include "physics.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

const int NOI = (int) (NOP / 2.0 * (NOP - 1)); // Number of interactions
const int NOT = NOI / NOIPT + (NOI % NOIPT > 0); // Number of threads

CombCouple* combArray;
InteractionData interactionDatas[NOT];

int initializeParticles(Particle* particles) {
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

    const int wholeThreads = NOI / NOIPT;
    const int partialThread = NOI % NOIPT > 0;

    for (int i = 0; i < wholeThreads; i++) {
        interactionDatas[i] = (InteractionData) {
            combArray + i * NOIPT,
            combArray + (i + 1) * NOIPT,
        };
    }

    if (partialThread) {
        interactionDatas[wholeThreads] = (InteractionData) {
            combArray + wholeThreads * NOIPT,
            combArray + NOI - 1,
        };
    }

    return 0;
}

void destroyParticles(Particle* particles) {
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

    for (CombCouple* particles = interactionData->startPointer; particles < interactionData->endPointer; particles++) {
        Particle* particleOne = particles->particleOne;
        Particle* particleTwo = particles->particleTwo;

        float diffX = particleOne->posX - particleTwo->posX;
        float diffY = particleOne->posY - particleTwo->posY;

        float distance = calcDistance(diffX, diffY);

        if (distance > 20) {
            float gravWeight = 1 / powThree(distance);

            float gravFactorI = -particleTwo->mass * gravWeight;
            float gravFactorJ = -particleOne->mass * gravWeight;

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

void updatePhysics(Particle* particles) {
    struct timeval t0;
    struct timeval t1;
    gettimeofday(&t0, 0);

    pthread_t threadIds[NOT];

    for (int i = 0; i < NOT; i++) {
        pthread_create(&threadIds[i], NULL, particleInteractions, (void*) (interactionDatas + i));
    }

    for (int i = 0; i < NOT; i++) {
        pthread_join(threadIds[i], NULL);
    }

    for (int i = 0; i < NOP; i++) {
        particles[i].posX += particles[i].velX;
        particles[i].posY += particles[i].velY;
    }

    gettimeofday(&t1, 0);
    printf("%f\n", timedifference_msec(t0, t1));
}
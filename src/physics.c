#include "physics.h"
#include "helperFiles/globalStructs.h"
#include "helperFiles/globalFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Particle* particles;
CombCouple* combArray;
InteractionData* interactionDatas;
thrd_t* threadIds;

int nop; // Number of particles
int noi; // Number of interactions
int not; // Number of threads

Particle* getParticles() {
    return particles;
}

int initializeParticles(int _nop) {
    // Setting const variables (not really contant)
    nop = _nop;
    noi = (int) (nop / 2.0 * (nop - 1));
    not = noi / NOIPT + (noi % NOIPT > 0);

    // Creating space for thread ids
    threadIds = (thrd_t*) malloc(sizeof(thrd_t) * not);

    // Creating Particles
    particles = (Particle*) malloc(sizeof(Particle) * nop);

    for (int i = 0; i < nop; i++) {
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

        mtx_init(&particles[i].mutex, mtx_plain);
    }

    // Creating combination array
    combArray = (CombCouple*) malloc(sizeof(CombCouple) * noi);

    if (combArray == NULL) {
        return 1;
    }

    int combIndex = 0;

    for (int i = 0; i < nop; i++) {
        for (int j = i + 1; j < nop; j++) {
            combArray[combIndex] = (CombCouple) {
                particles + i,
                particles + j,
            };

            combIndex++;
        }
    }


    // Creating interaction datas
    interactionDatas = (InteractionData*) malloc(sizeof(InteractionData) * not);
    
    const int wholeThreads = noi / NOIPT;
    const int partialThread = noi % NOIPT > 0;

    for (int i = 0; i < wholeThreads; i++) {
        interactionDatas[i] = (InteractionData) {
            combArray + i * NOIPT,
            combArray + (i + 1) * NOIPT,
        };
    }

    if (partialThread) {
        interactionDatas[wholeThreads] = (InteractionData) {
            combArray + wholeThreads * NOIPT,
            combArray + noi - 1,
        };
    }

    return 0;
}

void destroyParticles() {
    for (int i = 0; i < nop; i++) {
        mtx_destroy(&particles[i].mutex);
    }

    free(combArray);
    free(particles);
    free(interactionDatas);
    free(threadIds);
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

            mtx_lock(&particleOne->mutex);
            particleOne->velX += gravFactorI * diffX;
            particleOne->velY += gravFactorI * diffY;
            mtx_unlock(&particleOne->mutex);

            mtx_lock(&particleTwo->mutex);
            particleTwo->velX -= gravFactorJ * diffX;
            particleTwo->velY -= gravFactorJ * diffY;
            mtx_unlock(&particleTwo->mutex);
        }
    }

    return NULL;
}

void updatePhysics() {
    for (int i = 0; i < not; i++) {
        thrd_create(&threadIds[i], particleInteractions, (void*) (interactionDatas + i));
    }

    for (int i = 0; i < not; i++) {
        thrd_join(threadIds[i], NULL);
    }

    for (int i = 0; i < nop; i++) {
        particles[i].posX += particles[i].velX;
        particles[i].posY += particles[i].velY;
    }
}
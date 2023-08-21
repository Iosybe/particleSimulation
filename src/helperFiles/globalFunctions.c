#include "globalFunctions.h"

#include <stdlib.h>
#include <math.h>

int randInt(int max) {
    return rand() % max;
}

float randFloat(float max) {
    return 1 + (float) rand() * max / (float) RAND_MAX;
}

float randFloatRange(float min, float max) {
    return min + (float) rand() * (max - min) / (float) RAND_MAX;
}
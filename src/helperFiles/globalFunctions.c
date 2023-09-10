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

float powTwo(float n) {
    return n * n;
}

float powThree(float n) {
    return n * n * n;
}

float calcDistance(float diffX, float diffY) {
    return sqrt(powTwo(diffX) + powTwo(diffY));
}
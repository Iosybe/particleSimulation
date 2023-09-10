#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

// Generates random float that is between 1 and max + 1
float randFloat(float max);

// Generates random float between min and max
float randFloatRange(float min, float max);

// Optimization because pow() is slow
float powTwo(float n);
float powThree(float n);

float calcDistance(float diffX, float diffY);

#endif
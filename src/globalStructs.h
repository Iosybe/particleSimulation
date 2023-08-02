#ifndef GLOBALSTRUCTS_H
#define GLOBALSTRUCTS_H

typedef struct viewportStateStruct {
    float transX;
    float transY;

    float zoomScale;

    double prevPosX;
    double prevPosY;
} ViewportStateStruct;

#define viewportState = ViewportStateStruct {
    0.0, // transX
    0.0, // transY
    1.0, // zoomScale
    0.0, // prevPosX
    0.0 // prevPosY
};


#endif
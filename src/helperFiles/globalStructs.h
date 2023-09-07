#ifndef GLOBALSTRUCTS_H
#define GLOBALSTRUCTS_H

typedef struct viewportStateStruct {
    float transX;
    float transY;

    float zoomScale;

    double prevPosX;
    double prevPosY;

    double cursorZoomPosX;
    double cursorZoomPosY;

    int trackedParticle;
} ViewportStateStruct;

typedef struct windowStateStruct {
    int width;
    int height;

    int fullscreenState;
} WindowStateStruct;

typedef struct simulationStateStruct {
    int pauze;
} SimulationStateStruct;

extern ViewportStateStruct viewportState;
extern WindowStateStruct windowState;
extern SimulationStateStruct simulationState;

#endif



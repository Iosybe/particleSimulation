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

    int windowedWidth;
    int windowedHeight;

    int windowedPosX;
    int windowedPosY;

    int fullscreenState;
} WindowStateStruct;

extern ViewportStateStruct viewportState;
extern WindowStateStruct windowState;

#endif



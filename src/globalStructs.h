#ifndef GLOBALSTRUCTS_H
#define GLOBALSTRUCTS_H

typedef struct viewportStateStruct {
    float transX;
    float transY;

    float zoomScale;

    double prevPosX;
    double prevPosY;

    int fullscreenState;

    double cursorZoomPosX;
    double cursorZoomPosY;
} ViewportStateStruct;

typedef struct windowStateStruct {
    int width;
    int height;
} WindowStateStruct;

extern ViewportStateStruct viewportState;

#endif



#include "globalStructs.h"

ViewportStateStruct viewportState = {
    .transX = 0.0,
    .transY = 0.0,

    .zoomScale = 1.0,

    .prevPosX = 0.0,
    .prevPosY = 0.0,

    .cursorZoomPosX = 0,
    .cursorZoomPosY = 0,

    .trackedParticle = -1,
};

WindowStateStruct windowState = {
    .width = 500,
    .height = 500,
    
    .windowedWidth = 500, 
    .windowedHeight = 500,

    .windowedPosX = 0,
    .windowedPosY = 0,

    .fullscreenState = 0,
};

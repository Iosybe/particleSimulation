#include "globalStructs.h"

ViewportStateStruct viewportState = (ViewportStateStruct) {
    0.0,    // transX
    0.0,    // transY
    1.0,    // zoomScale
    0.0,    // prevPosX
    0.0,    // prevPosY
    0,      // cursorZoomPosX
    0,      // cursorZoomPosY
    -1,     // trackedParticle
};

WindowStateStruct windowState = (WindowStateStruct) {
    500, // width
    500, // height
    0, // fullscreenState
};

SimulationStateStruct simulationState = (SimulationStateStruct) {
    0,
};

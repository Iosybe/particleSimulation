#include "globalStructs.h"

void initializeStateStructs() {
    viewportState = (ViewportStateStruct) {
        0.0,    // transX
        0.0,    // transY
        1.0,    // zoomScale
        0.0,    // prevPosX
        0.0,    // prevPosY
        0,      // fullscreenState
        0,      // cursorZoomPosX
        0,      // cursorZoomPosY
    };


}
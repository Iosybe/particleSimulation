#include "glfwCallbacks.h"
#include "../helperFiles/globalStructs.h"
#include "../helperFiles/globalFunctions.h"
#include "../physics.h"

int callbacksNop = 0;

void initializeGlfwCallbacks(int _nop) {
    callbacksNop = _nop;
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    windowState.width = width;
    windowState.height = height;

    UNUSED(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    UNUSED(window);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &viewportState.prevPosX, &viewportState.prevPosY);
    }

    UNUSED(mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    viewportState.zoomScale *= 1.0 + yoffset * 0.1;    
    
    UNUSED(window);
    UNUSED(xoffset);
}

void cursor_position_callback(GLFWwindow* window, double cursorPosX, double cursorPosY) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        return;
    }

    viewportState.trackedParticle = -1;

    viewportState.transX += 2 * (cursorPosX - viewportState.prevPosX) / viewportState.zoomScale;
    viewportState.transY += 2 * (viewportState.prevPosY - cursorPosY) / viewportState.zoomScale;
    
    viewportState.prevPosX = cursorPosX;
    viewportState.prevPosY = cursorPosY;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* primaryMonitorMode = glfwGetVideoMode(primaryMonitor);
        
        if (windowState.fullscreenState) {
            glfwSetWindowMonitor(window, NULL, windowState.windowedPosX, windowState.windowedPosY, windowState.windowedWidth, windowState.windowedHeight, 0);
            windowState.fullscreenState = !windowState.fullscreenState;
        }
        else {
            windowState.windowedWidth = windowState.width;
            windowState.windowedHeight = windowState.height;

            int posX, posY;
            glfwGetWindowPos(window, &posX, &posY);

            windowState.windowedPosX = posX;
            windowState.windowedPosY = posY;

            glfwSetWindowMonitor(window, primaryMonitor, 0, 0, primaryMonitorMode->width, primaryMonitorMode->height, primaryMonitorMode->refreshRate);
            windowState.fullscreenState = !windowState.fullscreenState;
        }
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        viewportState.trackedParticle = randInt(callbacksNop);
    }

    if (key == GLFW_KEY_MINUS && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        viewportState.zoomScale *= 0.9;
    }

    if (key == GLFW_KEY_EQUAL && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        viewportState.zoomScale *= 1.1;
    }

    if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        simulationState.pauze = !simulationState.pauze;
    }

    if (key == GLFW_KEY_PERIOD && simulationState.pauze == 1 && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        updatePhysics();
    }

    UNUSED(mods);
    UNUSED(scancode);
}
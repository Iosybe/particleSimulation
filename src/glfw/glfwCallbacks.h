#ifndef GLFWCALLBACKS_H
#define GLFWCALLBACKS_H

#include <GLFW/glfw3.h>

void initializeGlfwCallbacks(int _NOP);
void window_size_callback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double cursorPosX, double cursorPosY);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


#endif 
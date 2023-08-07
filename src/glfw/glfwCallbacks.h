#ifndef GLFWCALLBACKS_H
#define GLFWCALLBACKS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double cursorPosX, double cursorPosY);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


#endif 
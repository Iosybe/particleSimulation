#ifndef LOADSHADER_H
#define LOADSHADER_H

#include <glad/glad.h>

GLuint LoadComputeShaders(const char* ComputeFilePath);
GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);

#endif
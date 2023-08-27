#include <stdio.h>
#include <stdlib.h>

#include "loadShader.h"

long getFileSize(FILE* file) {
	if (file == NULL) { return 1; }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return size;
}

char* getFileStr(const char* filePath) {
	FILE* file = fopen(filePath, "rb");
	if (file == NULL) { return NULL; };

    long fileSize = getFileSize(file);
	if (fileSize == 0) { return NULL; };

    char* fileStr = (char*) malloc(sizeof(char) * (fileSize + 1));

    for (int i = 0; i < fileSize; i++) {
		fileStr[i] = fgetc(file);
    }

	fileStr[fileSize] = '\0';
    fclose(file);

    return fileStr;
}

void checkCompileErrors(GLuint id, const char* file) {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
        char* computeShaderErrorMessage = (char*) malloc(sizeof(char) * (InfoLogLength + 1));
		glGetShaderInfoLog(id, InfoLogLength, NULL, &computeShaderErrorMessage[0]);

		printf("%s\n", file);
		printf("%s\n", &computeShaderErrorMessage[0]);
	}
}

GLuint LoadComputeShaders(const char* computeFilePath) {
	GLuint computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    // Read compute shader from file
    const char* computeStr = getFileStr(computeFilePath);

    if (computeStr == NULL) {
        printf("Failed to read %s\n", computeFilePath);
        return 0;
    }

	// Compile compute shader
	glShaderSource(computeShaderID, 1, &computeStr, NULL);
	glCompileShader(computeShaderID);
	checkCompileErrors(computeShaderID, computeFilePath);

	// Link the program
	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, computeShaderID);
	glLinkProgram(ProgramID);
	checkCompileErrors(ProgramID, computeFilePath);
	
	glDetachShader(ProgramID, computeShaderID);
	glDeleteShader(computeShaderID);

	return ProgramID;
}

GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath) {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read vertex shader from file
    const char* vertStr = getFileStr(vertexFilePath);

    if (vertStr == NULL) {
        printf("Failed to read %s\n", vertexFilePath);
        return 0;
    }

    // Read fragment shader from file
    const char* fragStr = getFileStr(fragmentFilePath);

    if (fragStr == NULL) {
        printf("Failed to read %s\n", fragmentFilePath);
        return 0;
    }

	// Compile vertex shader
	glShaderSource(vertexShaderID, 1, &vertStr, NULL);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID, vertexFilePath);

	// Compile fragment shader
	glShaderSource(fragmentShaderID, 1, &fragStr, NULL);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(vertexShaderID, fragmentFilePath);

	// Link the program
	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// combine the file paths
	checkCompileErrors(ProgramID, "vertex + fragment file");
	
	glDetachShader(ProgramID, vertexShaderID);
	glDetachShader(ProgramID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return ProgramID;
}
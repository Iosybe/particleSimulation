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

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile vertex shader
	glShaderSource(vertexShaderID, 1, &vertStr, NULL);
	glCompileShader(vertexShaderID);

	// Check vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
        char* vertexShaderErrorMessage = (char*) malloc(sizeof(char) * (InfoLogLength + 1));
		glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);

		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile fragment shader
	glShaderSource(fragmentShaderID, 1, &fragStr, NULL);
	glCompileShader(fragmentShaderID);

	// Check fragment shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
        char* fragmentShaderErrorMessage = (char*) malloc(sizeof(char) * (InfoLogLength + 1));
		glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);

		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program
	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
        char* programErrorMessage = (char*) malloc(sizeof(char) * (InfoLogLength + 1));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, vertexShaderID);
	glDetachShader(ProgramID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return ProgramID;
}
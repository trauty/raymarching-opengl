#pragma once

#include "glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string getFileContents(const char* filename);

class Shader
{
public:
	GLuint id;
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);

	void activate();
	void dispose();

private:
	void compileErrors(unsigned int shader, const char* type);
};
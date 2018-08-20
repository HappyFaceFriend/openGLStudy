#pragma once
#include "GLDebug.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char *file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << function << "/" << file << ":" << line << error << std::endl;
		return false;
	}
	return true;
}

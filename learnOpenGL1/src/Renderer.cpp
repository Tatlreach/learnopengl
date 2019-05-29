

#include "Renderer.h"
#include <iostream>

void GLClearError() {	//remove all existing error enums
	while (glGetError());
}

bool GLLogCall(const char* function, const char* file, int line) {	//print all existing error enums
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << '\n';
		return false;
	}

	return true;
}
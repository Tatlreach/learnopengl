#pragma once

#include <GL/glew.h>


void GLClearError();

///	Debug breaks at line if x is NULL.
#define ASSERT(x) if((!x)) __debugbreak();

/// Prints the function, file & line for all current GLenum errors ret by glGetError().
/// ret true if no errors.  
bool GLLogCall(const char* function, const char* file, int line);


///	Clears Errors.
///	Calls x.
///	ASSERT debug breaks if GLLogCall prints an error.
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))
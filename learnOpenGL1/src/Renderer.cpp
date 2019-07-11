

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

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	shader.Bind();
	va.Bind();		//already has a binding to ib
	ib.Bind();

	//draw what is bound
	//currently hardcoded as "GL_UNSIGNED_INT"
	//currently hardcoded as "GL_TRIANGLES"
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

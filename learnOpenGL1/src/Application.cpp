#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "VertexArray.h"
#include "Shader.h"


int main(void)
{

	GLFWwindow* window;

	//init library
	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//make the window the current context
	window = glfwCreateWindow(800, 600, "My Typed Up Triangle Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	//MUST BE AFTER CONTEXT
	if (glewInit() != GLEW_OK) {
		//print something
		//std::cout << "glewInit is not GLEW_OK" << '\n';
	}

	//std::cout << glGetString(GL_VERSION) << '\n';


	float positions[8] = {
		-.5f,	-.5f,
		-.5f,	0.5f,
		0.5f,	-.5f,
		0.5f,	0.5f,
	};

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 1
	};

	VertexArray va;

	//define vertex buffer
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));	//auto binds the vb


	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	/*
	GLCall(glEnableVertexAttribArray(0));
	//links buffer with VAO
	GLCall(glVertexAttribPointer(0,	//starting index of specified attribute
		2,						//number of components in ea. vertex
		GL_FLOAT,			//Attribute Type
		GL_FALSE,			//normalized
		sizeof(float) * 2,	//stride, total size of vertex
		0));					//offset for the attribute WITHIN the vertex e.g. (const void*)
*/

	//define index buffer
	IndexBuffer ib(indices, 6);

//	glBindBuffer(GL_ARRAY_BUFFER, 0);	//bind to empty buffer


	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	//requires bound shader
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	shader.Unbind();

	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	float r = 0.6f;
	float increment = 0.05f;	//loop until window closed
	while (!glfwWindowShouldClose(window)) {

		//RENDER
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLClearError();	//remove all existing error enums

		//change color every tick
		if ((r > 1.0f) || (r < 0.0f)) {		
			increment *= -1.0f;
		}
		r += increment;


		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		va.Bind();	//remove following line
		ib.Bind();


		//draw what is bound
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		//dont' have to reference the buffer because it's already bound

		
//		ASSERT(GLLogCall());	//print all existing error enums

		//swap front & back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	//GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}
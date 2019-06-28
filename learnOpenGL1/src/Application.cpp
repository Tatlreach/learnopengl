#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Texture.h"


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


	float positions[] = {
		-0.5f,	-0.5f,	0.0f, 0.0f,	// left bottom
		-0.5f,	0.5f,	0.0f, 1.0f,	// left top
		0.5f,	-0.5f,	1.0f, 0.0f,	// right bottom
		0.5f,	0.5f,	1.0f, 1.0f	// right top
	};

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 1
	};

	VertexArray va;

	//VertexBuffer(datal, size)		//auto binds the vb
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));	//4 floats per vertex


	VertexBufferLayout layout;
	layout.Push<float>(2);
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
	//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	Texture texture("res/textures/clover.png");
	//bind slot needs to match set uniform slot
	texture.Bind(0);
	shader.SetUniform1i("u_Texture", 0);	//0 has to match bound slot for texture

	// NULL binds all elements
	va.Unbind();
	vb.Unbind();
//	GLCall(glBindVertexArray(0));
//	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	ib.Unbind();
	//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	shader.Unbind();
//	GLCall(glUseProgram(0));

	Renderer renderer;

	float r = 0.6f;				//red val seed
	float increment = 0.05f;	//red val inc every refresh
	
	//loop until window closed
	while (!glfwWindowShouldClose(window)) {

		GLClearError();	//remove all existing error enums

		//adjust red val every tick
		if ((r > 1.0f) || (r < 0.0f)) {		
			increment *= -1.0f;
		}
		r += increment;


		shader.Bind();
		//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer.Draw(va, ib, shader);
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
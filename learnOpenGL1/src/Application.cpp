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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


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
	window = glfwCreateWindow(960, 540, "My Typed Up Triangle Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	//MUST BE AFTER CONTEXT
	if (glewInit() != GLEW_OK) {
		//print something
		//std::cout << "glewInit is not GLEW_OK" << '\n';
	}

	//std::cout << glGetString(GL_VERSION) << '\n';


	float positions[] = {
		-50.0f, -50.0f,	0.0f, 0.0f,	// left bottom
		-50.0f, 50.0f, 0.0f, 1.0f, // left top
		50.0f,	-50.0f,	1.0f, 0.0f,	// right bottom
		50.0f,	50.0f,	1.0f, 1.0f	// right top
	};

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 1
	};

	//setting how opengl blends alpha pixels
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

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
	//4x4 matrix
	//creates orthographic matrix,
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);	 //when multiplied, converts it to a space between -1 & 1

	//camera transformations are actually reverse, because we're acting on the world not the camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));	//creates an identity matrix (of 1s), translates x -100.0f



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

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	glm::vec3 translationA(400.0f, 200.0f, 0.0f);
	glm::vec3 translationB(200.0f, 200.0f, 0.0f);

	float r = 0.6f;				//red value seed
	float increment = 0.05f;	// increment the red value every refresh
	
	//loop until window closed
	while (!glfwWindowShouldClose(window)) {

		//GLClearError();	//remove all existing error enums
		renderer.Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		//adjust red val every tick
		if ((r > 1.0f) || (r < 0.0f)) {		
			increment *= -1.0f;
		}
		r += increment;
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(va, ib, shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);	//only diff vs the code above
			glm::mat4 mvp = proj * view * model;
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(va, ib, shader);
		}
//		ASSERT(GLLogCall());	//print all existing error enums

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
			ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}


		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		//swap front & back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}


	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
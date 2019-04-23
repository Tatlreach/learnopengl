#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <iostream>

int main(void)
{
	
	GLFWwindow* window;

	//init library
	if (!glfwInit()) {
		return -1;
	}

	//make the window the current context
	window = glfwCreateWindow(800, 600, "My Typed Up Triangle Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	//MUST BE AFTER CONTEXT
	if (glewInit() != GLEW_OK) {
		//print something
		//std::cout << "glewInit is not GLEW_OK" << '\n';
	}


	//std::cout << glGetString(GL_VERSION) << '\n';

	float positions[6] = {
		-.5f,	-.5f,
		0.0f,	0.5f,
		.5f,	-.5f
	};

	//define buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//fill it with data

	//loop until window closed
	while (!glfwWindowShouldClose(window)) {

		//RENDER
		glClear(GL_COLOR_BUFFER_BIT);

		//swap front & back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
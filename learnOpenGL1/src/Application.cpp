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

	//loop until window closed
	while (!glfwWindowShouldClose(window)) {

		//RENDER
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(-.5f, -.5f);
		glVertex2f(0.0f, .5f);
		glVertex2f(.5, -.5f);
		
		glEnd();

		//swap front & back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
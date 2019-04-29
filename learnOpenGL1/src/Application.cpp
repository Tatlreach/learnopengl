#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string source ) {
	unsigned int id = glCreateShader(GL_VERTEX_SHADER);	//creates empty shader program to hold string src code
	const char* src = source.c_str();	//return pointer to null terminated mutable array
	glShaderSource(id, 1, &src, nullptr);	//Replaces source code in shader object
	glCompileShader(id);

	//TODO: Error Handling

	return id;
}

//returns an ID for the shader
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//each of these are program IDs
	//can use GLuint  for type too
	unsigned int program = glCreateProgram();		//creates empty program to contain shaders
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader );
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//link 2 shaders into 1 file
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

}

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
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);	//fill it with data


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,	//starting index of specified attribute
		2,						//number of components in ea. vertex
		GL_FLOAT,			//Attribute Type
		GL_FALSE,			//normalized
		sizeof(float)*2,	//stride, total size of vertex
		0);					//offset for the attribute WITHIN the vertex e.g. (const void*)8


	glBindBuffer(GL_ARRAY_BUFFER, 0);	//bind to empty buffer


	//loop until window closed
	while (!glfwWindowShouldClose(window)) {

		//RENDER
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap front & back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
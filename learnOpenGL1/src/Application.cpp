#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	
	std::stringstream ss[2];
	std::string line;
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				//set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				//set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			//push line to specific type of stringstream
			ss[(int)type] << line << '\n';
		}

	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string source ) {
	unsigned int id = glCreateShader(type);	//creates empty shader program to hold string src code
	const char* src = source.c_str();	//return pointer to null terminated mutable array
	glShaderSource(id, 1, &src, nullptr);	//Replaces source code in shader object
	glCompileShader(id);				//compiles the string code in the shader


	//Error Handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);	//i: integer, v: vector //query the compile to see errors

	if (result == GL_FALSE) {
		//did not compile
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile " << (type==GL_VERTEX_SHADER? "vertex" : "fragment") <<" shader" << '\n';
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

//returns an ID for the shader
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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
	glValidateProgram(program);		//checks if program is runnable given state, if succeeds GL_TRUE is stored as part of the program object's state


	//glDetachShader properly deletes source code
	//glDetachShader right away removes a lot of debugging options
	//deletes the intermediates, actual
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
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
		sizeof(float) * 2,	//stride, total size of vertex
		0);					//offset for the attribute WITHIN the vertex e.g. (const void*)8


	glBindBuffer(GL_ARRAY_BUFFER, 0);	//bind to empty buffer

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	//std::cout << "#VERTEX" << '\n';
	//std::cout << source.VertexSource << '\n' << '\n';
	//std::cout << "#FRAGMENT" << '\n';
	//std::cout << source.FragmentSource << std::endl;

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

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
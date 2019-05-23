#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if((!x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))


static void GLClearError() {	//remove all existing error enums
	while (glGetError());
}

static bool GLLogCall(const char* function, const char* file, int line) {	//print all existing error enums
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file <<": "<< line << '\n';
		return false;
	}

	return true;
}

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
	GLCall(glShaderSource(id, 1, &src, nullptr));	//Replaces source code in shader object
	GLCall(glCompileShader(id));				//compiles the string code in the shader


	//Error Handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));	//i: integer, v: vector //query the compile to see errors

	if (result == GL_FALSE) {
		//did not compile
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*) alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "failed to compile " << (type==GL_VERTEX_SHADER? "vertex" : "fragment") <<" shader" << '\n';
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
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
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));		//checks if program is runnable given state, if succeeds GL_TRUE is stored as part of the program object's state


	//glDetachShader properly deletes source code
	//glDetachShader right away removes a lot of debugging options
	//deletes the intermediates, actual
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

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

	int indices[6] = {
		0, 1, 2,
		2, 3, 1
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	//define vertex buffer
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));	//fill it with data


	GLCall(glEnableVertexAttribArray(0));
	//links buffer with VAO
	GLCall(glVertexAttribPointer(0,	//starting index of specified attribute
		2,						//number of components in ea. vertex
		GL_FLOAT,			//Attribute Type
		GL_FALSE,			//normalized
		sizeof(float) * 2,	//stride, total size of vertex
		0));					//offset for the attribute WITHIN the vertex e.g. (const void*)8


	//define index buffer
	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW));	//fill it with data


//	glBindBuffer(GL_ARRAY_BUFFER, 0);	//bind to empty buffer

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	//requires bound shader
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT((location != -1));		//if -1, uniform wasn't found
	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

	//std::cout << "#VERTEX" << '\n';
	//std::cout << source.VertexSource << '\n' << '\n';
	//std::cout << "#FRAGMENT" << '\n';
	//std::cout << source.FragmentSource << std::endl;

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

		if ((r > 1.0f) || (r < 0.0f)) {
			increment *= -1.0f;
		}
		r += increment;

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		//dont' have to reference the buffer because it's already bound

		
//		ASSERT(GLLogCall());	//print all existing error enums

		//swap front & back buffers
		glfwSwapBuffers(window);

		//poll for and process events
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}
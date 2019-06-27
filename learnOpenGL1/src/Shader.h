#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {

private:
	std::string m_FilePath;	//for debug purposes
	unsigned int m_RendererID;
	//caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	/// "ParseShader" on txt file for shader source
	/// "CreateShader" on sources
	Shader(const std::string& filepath);
	~Shader();



	void Bind() const;
	void Unbind() const;

	/// uses glUniform4f(...) to set a "uniform vec4 name" in the shader
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f0, float f1);

private:
	///	Parses the text files for vertex & fragment tags.  
	///	Makes an object containing these source strings.
	ShaderProgramSource ParseShader(const std::string & filepath);

	/// Calls 'CompileShader' on arguments.  
	/// Creates a program,  attaches both shaders,  links program, & validates 
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	///	Creates a shader from source.  
	///	Compiles the shader.
	unsigned int CompileShader(unsigned int type, const std::string source);
	int GetUniformLocation(const std::string& name);

};
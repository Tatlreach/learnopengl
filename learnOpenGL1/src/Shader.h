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
	Shader(const std::string& filepath);
	~Shader();



	void Bind() const;
	void Unbind() const;

	//!set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float f0, float f1);

private:

	
	///	parses the text files for vertex & fragment tags.  
	///	makes an object containing these source strings
	ShaderProgramSource ParseShader(const std::string & filepath);


	///calls 'CompileShader' on arguments
	///attaches, links & validates a created program
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	///	creates a shader from source.  
	///	compiles the shader
	unsigned int CompileShader(unsigned int type, const std::string source);
	unsigned int GetUnifromLocation(const std::string& name);

};
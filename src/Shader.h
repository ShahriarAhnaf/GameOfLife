#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shaders
{
private:
	std::string m_file_path;
	unsigned int m_Renderer_ID;
	std::unordered_map<std::string, int> m_uniform_location_cache;
	//caching uniforms
public:
	Shaders(const std::string& filepath);
	~Shaders();
	void Bind() const;
	void unBind() const;

	//set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float f3);
private:
	static ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int GetUniformLocation(const std::string& name);
};


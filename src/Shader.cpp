#include "Shader.h"


Shaders::Shaders(const std::string& filepath)
	:m_file_path(filepath), m_Renderer_ID(0)
{

	//parse the shaders
	ShaderProgramSource source_shader = ParseShader(filepath); \
		// SHADER
		GLcall(m_Renderer_ID = CreateShader(source_shader.VertexSource, source_shader.FragmentSource));
}
Shaders:: ~Shaders() {
	GLcall(glDeleteProgram(m_Renderer_ID));
}


void Shaders::Bind() const {
	GLcall(glUseProgram(m_Renderer_ID));
}
void Shaders::unBind() const {

	GLcall(glUseProgram(0));
}

ShaderProgramSource Shaders::ParseShader(const std::string& filePath) {
	std::ifstream stream(filePath);
	std::stringstream ss[2];
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;
	std::string line;
	// parse through the lines
	while (getline(stream, line)) {
		// chekc the shader
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}


unsigned int Shaders::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	GLcall(unsigned int program = glCreateProgram());
	unsigned int Vshader = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int Fshader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, Vshader);
	glAttachShader(program, Fshader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(Vshader);
	glDeleteShader(Fshader);
	return program;
}

unsigned int Shaders::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* raw_src = source.c_str();
	glShaderSource(id, 1, &raw_src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
	}

	return id;
}

//set uniforms
void Shaders::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	GLcall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3))
}
unsigned int Shaders::GetUniformLocation(const std::string& name)
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end()) {
		return m_uniform_location_cache[name];
	}
	else {
		GLcall(int location = glGetUniformLocation(m_Renderer_ID, name.c_str()));
		if (location == -1) {
			std::cout << "warning uniform " << name << "doesnt existo!" << std::endl;
		}
		m_uniform_location_cache[name] = location;

		return location;
	}
}

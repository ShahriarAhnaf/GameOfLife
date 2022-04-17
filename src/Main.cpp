
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Game.h"



//GLOBALS
int window_width = 800;
int window_height = 600;








struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};


static ShaderProgramSource ParseShader(const std::string& filePath){
	std::ifstream stream(filePath);
	std::stringstream ss[2];
	enum class ShaderType {
			NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;
	std::string line;
	// parse through the lines
	while(getline(stream, line)){
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

static unsigned int CompileShader( unsigned int type, const std::string& source)
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
		std::cout << "failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader" << std::endl;
		std::cout << message << std::endl;
	}

	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

float normalize_pixel_x(float pixel_pos) {
	// find the zero pos
	float zero_pos= (float)window_width / 2;
	return (pixel_pos - zero_pos) / zero_pos;
}

float normalize_pixel_y(float pixel_pos) {
	// find the zero pos
	float zero_pos = (float)window_height / 2;
	return -1*(pixel_pos - zero_pos) / zero_pos;
}


int main() {
	glfwInit(); // start glffw

	//setting version 3.3 MAJOR.MINOR
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//shows to glfw that we are using the modern core versiono of library
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	//pointer to created window in glfw
	GLFWwindow* window = glfwCreateWindow(window_width,window_height, "Triangle Sim", NULL, NULL);
	if (window == NULL) {
		std::cout << "we choked at creating window lel" << std::endl;
		glfwTerminate();
		return -1; // end program error.
	}



	// tell glfw thaht we want to tuse the window
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	// context is the current "holder" of what to do

	//use glad for grrafix

	gladLoadGL();

	// where we want open GL to run. 
	glViewport(0, 0, window_width, window_height);


	unsigned int size_x = 10;
	unsigned int size_y = 10;

	Game game(size_x, size_y);
	// put the board into vertices.

	float cell_size_x_in_pixels = (float)window_width / size_x;
	float cell_size_y_in_pixels = (float)window_height / size_y;
	unsigned int floats_in_pixel = 8; // 4 xy positions
	size_t size_all = (size_t)game.GetRows() * (size_t)game.GetColumns() * (size_t)floats_in_pixel;

	float* vertices = new float[size_all]{};
	/*
		Vertices[] = {
		x, y, // top left
		x, y, 
		x, y, 
		x, y,
		alive/dead,
		....
		}
		
	
	*/
	unsigned int count=0;
	for (unsigned int x = 0; x < size_x; x ++) {
		for (unsigned int y = size_y; y > 0; y--) { // flip Y because bottom left is now 0,0

			// find four pixels
			 // bottom left
			vertices[count] = normalize_pixel_x( x * cell_size_x_in_pixels);
			vertices[count +1] = normalize_pixel_y( y * cell_size_y_in_pixels);
			std::cout << "x : " << vertices[count] << " y :  " << vertices[count + 1] << std::endl;
			// bottom right
			vertices[count +2] = normalize_pixel_x( (x + 1) * cell_size_x_in_pixels);
			vertices[count +3] = normalize_pixel_y( y * cell_size_y_in_pixels);
			std::cout << "x : " << vertices[count +2] << " y :  " << vertices[count + 3] << std::endl;
			// top right
			vertices[count +4] = normalize_pixel_x( (x + 1) * cell_size_x_in_pixels);
			vertices[count +5] = normalize_pixel_y( (y - 1) * cell_size_y_in_pixels);
			std::cout << "x : " << vertices[count + 4] << " y :  " << vertices[count + 5] << std::endl;
			//top left
			vertices[count + 6] = normalize_pixel_x( (x) * cell_size_x_in_pixels);
			vertices[count + 7] = normalize_pixel_y( (y -1 ) * cell_size_y_in_pixels);
			std::cout << "x : " << vertices[count + 6] << " y :  " << vertices[count + 7] << std::endl;
				//game.board_value(x, y);
			count += 8;
		}
	}
	std::cout << "\n added " << count << " number of vertices \n" << "wit a size of " << sizeof(vertices);

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	// MAKE THE BUFFER and bind
	{		
		//create a buffer reference so taht you can store more data
		
		VertexArray VAO;
		// MAKE VAO BEFORE VB
		// make buffer
		GLcall(VertexBuffer VBO(vertices, sizeof(vertices)));
		VertexBufferLayout layout;
		GLcall(layout.Push<float>(2));
		GLcall(VAO.AddBuffer(VBO, layout));
		// index buffer for the elements to draw multiple shapes
		IndexBuffer index_buffer(indices, sizeof(indices));


		//parse the shaders
		ShaderProgramSource source_shader = ParseShader("res/shaders/Basic.shader"); \
			// SHADER
			GLcall(GLuint shader = CreateShader(source_shader.VertexSource, source_shader.FragmentSource));
		//bind the shader for program
		glUseProgram(shader);

		// colouring dis hoe

		GLcall(int location = glGetUniformLocation(shader, "uColor"));
		ASSERT(location != -1);
		float r = 0.5f;
		float increment = 0.1f;


		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);
			GLcall(glUniform4f(location, r, 0.0f, 0.2f, 1.0f));
			// dray the triangle
			GLcall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			//glDrawArrays(GL_TRIANGLES, 0, 3);
			if (r > 1.0f) {
				increment = -0.03f;
			}
			else if (r < 0.1f) {
				increment = 0.2f;
			}
			r = r * (1 + increment);
			glfwSwapBuffers(window);


			glfwPollEvents(); // queue all the thinngs.
	}


	// delete the uffers and objects that are still left over. 
}
	glfwDestroyWindow(window);


	glfwTerminate(); // end glfw
	return 0;
}

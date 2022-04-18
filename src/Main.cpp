
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
int window_width = 1000;
int window_height = 720;








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
	GLcall(glValidateProgram(program));

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
	GLFWwindow* window = glfwCreateWindow(window_width,window_height, "Game of Life", NULL, NULL);
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


	unsigned int size_x = 5;
	unsigned int size_y = 5;

	Game game(size_x, size_y);
	// put the board into vertices.

	const float cell_size_x_in_pixels = (float)window_width / size_x;
	const float cell_size_y_in_pixels = (float)window_height / size_y;
	const size_t maxVertexCount = (size_t)game.GetRows() * (size_t)game.GetColumns() * 4; // 4 xy positions  times lenght and width
	const size_t maxQuadCount = (size_t)size_x * (size_t)size_y;
	const size_t maxIndexCount = maxQuadCount * 6;
	float* vertices = new float[maxVertexCount*2]{};
	/*
		Vertices[] = {
		x, y, // top left
		x, y, 
		x, y, 
		x, y,
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
			// bottom right
			vertices[count +2] = normalize_pixel_x( (x + 1) * cell_size_x_in_pixels);
			vertices[count +3] = normalize_pixel_y( y * cell_size_y_in_pixels);
			// top right
			vertices[count +4] = normalize_pixel_x( (x + 1) * cell_size_x_in_pixels);
			vertices[count +5] = normalize_pixel_y( (y - 1) * cell_size_y_in_pixels);
			//top left
			vertices[count + 6] = normalize_pixel_x( (x) * cell_size_x_in_pixels);
			vertices[count + 7] = normalize_pixel_y( (y -1 ) * cell_size_y_in_pixels);
				//game.board_value(x, y);
			count += 8;
		}
	}
	//std::cout << "\n added " << count << " number of vertices \n" << "wit a size of " << sizeof(vertices);

	//std::cout << sizeof(*vertices);
	unsigned int* indices = new unsigned int[maxIndexCount] {	};
	unsigned int offset = 0;
	for (size_t i = 0; i < maxIndexCount; i += 6) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;
		offset += 4;
	}
	for (size_t i = 0; i < maxIndexCount; i ++) {
		std::cout << " indices " << indices[i] << " i : " << i << std::endl;
	}

	// MAKE THE BUFFER and bind
	{		
		//create a buffer reference so taht you can store more data
		
		GLcall(VertexArray VAO);
		// MAKE VAO BEFORE VB
		// make buffer
		GLcall(VertexBuffer VBO(vertices, sizeof(float) * maxVertexCount*2));
		VertexBufferLayout layout;
		GLcall(layout.Push<float>(2));
		GLcall(VAO.AddBuffer(VBO, layout));
		// index buffer for the elements to draw multiple shapes
		GLcall(IndexBuffer index_buffer(indices, maxIndexCount));


		//parse the shaders
		ShaderProgramSource source_shader = ParseShader("res/shaders/Basic.shader"); \
			// SHADER
		GLcall(GLuint shader = CreateShader(source_shader.VertexSource, source_shader.FragmentSource));
		//bind the shader for program
		glUseProgram(shader);

		// colouring dis hoe

		GLcall(int location = glGetUniformLocation(shader, "uColor"));
		ASSERT(location != -1);
		while (!glfwWindowShouldClose(window)) {
			game.run_frame(); //update the simulation
			glClear(GL_COLOR_BUFFER_BIT);

			for (unsigned int y = 0; y < game.GetRows(); y++) {
				for (unsigned int x = 0; x < game.GetColumns(); x++) {
					if (game.board_value(x, y)) {
						GLcall(glUniform4f(location, 1.0f, 0.0f, 0.2f, 1.0f));
					}
					else {
						GLcall(glUniform4f(location, 0.0f, 0.0f, 0.2f, 1.0f));
					}
					// draw the board cell square, equivalenet to 
					// two triangles or 4 elements or 8 float points.
					// offset using the pointer in the indices
					// address of indices * size of the indice * offset of each round(4 ) * current index of board
					unsigned long long index = (unsigned long long)y * (unsigned long long)game.GetRows() + (unsigned long long)x;
					unsigned long long offset =sizeof(*indices) * index * 6; 
					std::cout << indices + offset << " value : " << *(indices + offset) << " should be " << indices[index*4]<< std::endl;
					GLcall(glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, (const void*)(indices + offset)));
					//glDrawArrays(GL_TRIANGLES, 0, 3)
				}
			}
			glfwSwapBuffers(window);


			glfwPollEvents(); // queue all the thinngs.
	}


	// delete the uffers and objects that are still left over. 
}
	glfwDestroyWindow(window);


	glfwTerminate(); // end glfw
	return 0;
}

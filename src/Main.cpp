
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Game.h"
#include "Shader.h"


//GLOBALS
int window_width = 1000;
int window_height = 720;


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


	unsigned int size_x, size_y, letime = 0;

	std::cout << "WELCOME TO THE SIMULATION OF LIFE!!" << std::endl;

	std::cout << "how big do you want your game to be in width?" << std::endl;
	std::cin >> size_x;

	std::cout << "how big do you want your game to be in height?" << std::endl;
	std::cin >> size_y;

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
	/*
	FOR TESTING INDEX
	for (size_t i = 0; i < maxIndexCount; i++) {
		std::cout << " indices " << indices[i] << " i : " << i << std::endl;
	}*/

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
		Shaders shade("res/shaders/Basic.shader");
		shade.Bind();

		// colouring dis hoe
		count = 0;
		while (!glfwWindowShouldClose(window)) {
			//update the simulation
			glClear(GL_COLOR_BUFFER_BIT);
			//GLcall(glUniform4f(location, 1.0f, 0.0f, 0.2f, 1.0f));
			
			for (unsigned int y = 0; y < game.GetRows(); y++) {
				for (unsigned int x = 0; x < game.GetColumns(); x++) {
					if (game.board_value(x, y)) {
						shade.SetUniform4f("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
					}
					else {
						shade.SetUniform4f("uColor", 0.0f, 0.5f, 0.0f, 1.0f);
					}
					// draw the board cell square, equivalenet to 
					// two triangles or 4 elements or 8 float points.
					// offset using the pointer in the indices
					// address of indices * size of the indice * offset of each round(4 ) * current index of board
					unsigned long long index = (unsigned long long)y * (unsigned long long)game.GetRows() + (unsigned long long)x;
					unsigned long long offset = 4 * index; // 6 is the offset of each indice to the next quad
					//std::cout << (const void*)(indices + offset) << " value : " << *(indices + offset)<< std::endl;
					GLcall(glDrawElementsBaseVertex(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr, offset));
					
					//glfwSwapBuffers(window);
				}
			}
			//GLcall(glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (nullptr), 4));
			//GLcall(glDrawArrays(GL_TRIANGLES, 0, 3));
			//count++;
			game.run_frame();
			glfwSwapBuffers(window);


			glfwPollEvents(); // queue all the thinngs.
	}

	
	// delete the uffers and objects that are still left over. 
	}
	glfwDestroyWindow(window);


	glfwTerminate(); // end glfw
	return 0;
}

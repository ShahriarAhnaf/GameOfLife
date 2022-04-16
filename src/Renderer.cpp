#include "Renderer.h"

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "Open GL eRROR CODE : [" << error << "]"
			<< "in function: " << function << " at line" << line << " " << file << std::endl;
		return false;
	}
	return true;
}
#include "VertexBuffer.h"
#include "Renderer.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
//	glGenVertexArrays(1, &m_Renderer_ID);
	// MAKE THE BUFFERS
	GLcall(glGenBuffers(1, &m_Renderer_ID));
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID));
	GLcall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLcall(glDeleteBuffers(1, &m_Renderer_ID));
}

void VertexBuffer::Bind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_ID));
}

void VertexBuffer::unBind() const
{
	GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

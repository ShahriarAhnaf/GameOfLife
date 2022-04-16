#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	GLcall(glGenBuffers(1, &m_Renderer_ID));
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
	GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLcall(glDeleteBuffers(1, &m_Renderer_ID));
}

void IndexBuffer::Bind() const
{
	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
}

void IndexBuffer::unBind() const
{

	GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
